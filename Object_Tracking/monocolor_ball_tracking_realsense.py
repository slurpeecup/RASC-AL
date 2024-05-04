from collections import deque
from imutils.video import VideoStream
import pyrealsense2 as rs
import numpy as np
import argparse
import cv2
import imutils
import csv
import math
import serial
import time

'''
Configure program args
'''
def configureArgs():    
    global args 

    # configure arguments
    # -h/--help is automatically generated
    ap = argparse.ArgumentParser()
    ap.add_argument("-d", "--draw", action='store_true', help="draw to video stream")
    ap.add_argument("-b", "--buffer", type=int, default=0, help="max buffer size/tail length")
    ap.add_argument("-c", "--camera", type=int, default=0, help="camera device index")
    ap.add_argument("-r", "--radius", type=int, default=5, help="minimum radius of target")
    ap.add_argument("-w", "--width", type=int, default=1280, help="set video resize width")
    ap.add_argument("-o", "--output", default='tracking_output.csv', help="output file name")
    ap.add_argument("-V", "--verbose", action='store_true', help="output coordinates of target to command line") 
    ap.add_argument("-u", "--uart", default='/dev/ttyUSB0', help="path to the uart serial device")

    args = vars(ap.parse_args())

'''
Configure serial for UART
'''
def configureSer():
    global ser

    # UART with pyserial
    ser = serial.Serial(args["uart"], baudrate = 38400, bytesize = serial.EIGHTBITS, parity = serial.PARITY_NONE, stopbits = 1)
    ser.reset_input_buffer()


'''
Configure and start realsense camera stream
'''
def startVideoStream():
    global vs

    vs = rs.pipeline()
    config = rs.config()
    config.enable_stream(rs.stream.color, 1280, 720, rs.format.bgr8, 15)
    # Start stream
    vs.start(config)

'''
Get singular frame from stream
'''
def getFrame():
    return np.asanyarray(vs.wait_for_frames().get_color_frame().get_data())

'''
Calculate center of modified stream based on resized frame
'''
def setFrameCenter():
    global frameCenter
    frameCenter = (np.array(imutils.resize(getFrame(), width=args["width"]).shape[:2])//2)[::-1]

'''
Define color boundaries and buffers for ball markers
'''
def defineBoundaries():
    global colors
    global bgr
    global boundaries 
    global buffers

    # Define the color you want to track
    target_color = "red"

    # Set the RGB color for the target color (you can choose any color here, it's just for visualization)
    bgr_target = (0, 35, 255)  # Red

    # Define HSV boundaries for cherry red
    lower_bound = (162, 145, 134)  # Lower HSV threshold for cherry red
    upper_bound = (182, 255, 255)  # Upper HSV threshold for cherry red


    # Create a buffer for the target color
    target_buffer = deque(maxlen=args["buffer"])

    # Set boundaries and buffer for the target color
    boundaries = [(lower_bound, upper_bound)]
    buffers = [target_buffer]
    
    # Set colors and BGR values
    colors = [target_color]
    bgr = [bgr_target]


'''
Build string to be output to UART dx,d
'''
def buildOutputStr(coordData):
    return str(coordData[0][0]) + "," + str(coordData[0][1])

'''
This program starts a video stream via realsense camera and tracks the center of three balls
'''
def main():
    configureArgs()
    configureSer()
    startVideoStream()
    setFrameCenter()
    defineBoundaries()

    # Set the minimum area and circularity thresholds for a valid contour
    min_area = 100
    min_circularity = 0.7

    # set sysref time
    start_time = time.time()
    # initialize list of data points to be saved to csv or xmitted
    data = []

    # keep looping until video source stops
    while True:

        # grab the current frame
        frame = getFrame()

        # resize the frame, blur it, and convert it to the HSV color space
        frame = imutils.resize(frame, width=args["width"])
        blurred = cv2.GaussianBlur(frame, (11, 11), 0)
        hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

        # construct a mask for the target color, then perform
        # a series of dilations and erosions to remove any small
        # blobs left in the mask
        masks = [None] * len(colors)
        frameData = [[0, 0]] * len(colors)

        for i in range(len(colors)):
            masks[i] = cv2.inRange(hsv, boundaries[i][0], boundaries[i][1])
            masks[i] = cv2.erode(masks[i], None, iterations=2)
            masks[i] = cv2.dilate(masks[i], None, iterations=2)

            # find contours in the mask
            contours = cv2.findContours(masks[i].copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            contours = imutils.grab_contours(contours)
            center = None

            # loop over the contours
            for contour in contours:
                # compute the area and circularity of the contour
                area = cv2.contourArea(contour)
                perimeter = cv2.arcLength(contour, True)
                circularity = 4 * math.pi * area / (perimeter * perimeter)

                # check if the contour is a valid sphere
                if area > min_area and circularity > min_circularity:
                    # find the enclosing circle and centroid
                    ((x, y), radius) = cv2.minEnclosingCircle(contour)
                    center = (int(x), int(y))

                    # save dx, dy of center of contour from center of frame to data list
                    frameData[i] = center

                    if args.get("draw", True):
                        # update the buffer queue
                        buffers[i].appendleft(center)

                        # draw the circle and centroid on the frame
                        cv2.circle(frame, (int(x), int(y)), int(radius), bgr[i], 2)
                        cv2.circle(frame, center, int(args["radius"]), bgr[i], -1)

        # logic for detecting center of balls
        xSum = 0
        ySum = 0
        count = 0
        for x, y in frameData:
            if x > 0 or y > 0:
                count = count + 1
                xSum = xSum + x
                ySum = ySum + y

        hysteresis_trigger_x = 0
        hysteresis_trigger_y = 0
        if count > 0:
            triCenter = [(xSum // count), (ySum // count)]
            data.append([np.subtract(triCenter, frameCenter), count])

            dx, dy = data[-1][0]

            if abs(dx) <= 10:
                hysteresis_trigger_x = 1
            if abs(dy) <= 10:
                hysteresis_trigger_y = 1

            if dx < 0 and dy > 0:  # top right
                output_x = b'\x58\x63'  # Xc
                output_y = b'\x59\x6E'  # Yn
            if dx < 0 and dy < 0:
                output_x = b'\x58\x63'  # Xc
                output_y = b'\x59\x63'  # Yc
            if dx > 0 and dy > 0:
                output_x = b'\x58\x6E'  # Xn
                output_y = b'\x59\x6E'  # Yn
            if dx > 0 and dy < 0:
                output_x = b'\x58\x6E'  # Xn
                output_y = b'\x59\x63'  # Yc

            if hysteresis_trigger_x == 1 and abs(dx) > 50:
                hysteresis_trigger_x = 0
            if hysteresis_trigger_y == 1 and abs(dy) > 50:
                hysteresis_trigger_y = 0

            # tx/rx arduino
            if time.time() - start_time >= 0.02:
                if hysteresis_trigger_x == 0:
                    ser.write((output_x + b'\x02' + b'\x00'))  # //WTFFFF? python force inverting bits, undoing here
                time.sleep(0.01)
                if hysteresis_trigger_y == 0:
                    ser.write((output_y + b'\x02' + b'\x00'))
                start_time = time.time()

            if args.get("verbose", True):
                print(dx + dy)
                print("\n")
            # draw circle for middle of 3 balls
            cv2.circle(frame, triCenter, int(args["radius"]), (0, 0, 255), -1)

        # draw tails
        # loop over the set of tracked points
        if args.get("draw", True):
            for j in range(len(colors)):
                for i in range(1, len(buffers[j])):
                    # if either of the tracked points are None, ignore
                    # them
                    if buffers[j][i - 1] is None or buffers[j][i] is None:
                        continue
                    # otherwise, compute the thickness of the line and
                    # draw the connecting lines
                    thickness = int(np.sqrt(args["buffer"] / float(i + 1)) * 2.5)
                    hl, sl, vl = boundaries[j][0]
                    cv2.line(frame, buffers[j][i - 1], buffers[j][i], bgr[j], thickness)

        # show the frame to our screen
        cv2.imshow("Ball Tracking", frame)
        key = cv2.waitKey(1) & 0xFF

        # if the 'q' key is pressed, stop the loop
        if key == ord("q"):
            print("Closing serial pipe...")
            ser.close()
            break

    # stop the camera video stream
    print("Stopping video stream...")
    vs.stop()

    # close all windows
    cv2.destroyAllWindows()

    # output data to csv file
    print("Writing data to " + args["output"] + "...")
    with open(args["output"], 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(data)
    print("Done!")

if __name__ == "__main__":
    main()
