import cv2
import mediapipe as mp
import pyautogui
import time

# Disable PyAutoGUI's fail-safe mechanism (use cautiously!)
pyautogui.FAILSAFE = False

# Initialize MediaPipe Hand model
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.7)
mp_draw = mp.solutions.drawing_utils

# Initialize webcam
cap = cv2.VideoCapture(1)
screen_width, screen_height = pyautogui.size()

# Variables to store rectangle boundaries
rectangle_defined = False
top_left = None
bottom_right = None

# Threshold for detecting gestures (right click and left click)
GESTURE_THRESHOLD = 20
last_click_time = time.time()

def get_distance(point1, point2):
    return ((point1[0] - point2[0]) ** 2 + (point1[1] - point2[1]) ** 2) ** 0.5

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Flip the frame horizontally for a mirror-like view
    frame = cv2.flip(frame, 1)
    frame_height, frame_width, _ = frame.shape

    # Convert to RGB
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    result = hands.process(rgb_frame)

    if result.multi_hand_landmarks:
        for hand_landmarks in result.multi_hand_landmarks:
            mp_draw.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

            # Get coordinates of landmarks
            landmarks = hand_landmarks.landmark
            index_finger = landmarks[8]  # Index finger tip
            thumb_tip = landmarks[4]  # Thumb tip
            middle_finger = landmarks[12]  # Middle finger tip

            # Convert normalized coordinates to pixel values
            ix, iy = int(index_finger.x * frame_width), int(index_finger.y * frame_height)
            tx, ty = int(thumb_tip.x * frame_width), int(thumb_tip.y * frame_height)
            mx, my = int(middle_finger.x * frame_width), int(middle_finger.y * frame_height)

            # Detect gestures to define rectangle (touch thumb and index finger)
            if get_distance((ix, iy), (tx, ty)) < GESTURE_THRESHOLD:
                if not top_left:
                    top_left = (ix, iy)  # Set the top-left corner
                elif not bottom_right:
                    bottom_right = (ix, iy)  # Set the bottom-right corner
                    rectangle_defined = True
                    print("Rectangle defined!")
                # Right-click gesture
                pyautogui.click(button='right')
                last_click_time = time.time()

            # Left-click gesture (index and middle finger)
            if get_distance((ix, iy), (mx, my)) < GESTURE_THRESHOLD:
                if time.time() - last_click_time > 0.5:  # Prevent double-clicking too fast
                    pyautogui.click(button='left')
                    last_click_time = time.time()

            # Draw the rectangle
            if top_left:
                cv2.circle(frame, top_left, 10, (0, 255, 0), -1)  # Mark top-left corner
            if bottom_right:
                cv2.circle(frame, bottom_right, 10, (0, 255, 0), -1)  # Mark bottom-right corner
                cv2.rectangle(frame, top_left, bottom_right, (255, 0, 0), 2)

            # If the rectangle is defined, map cursor movements
            if rectangle_defined:
                rect_width = abs(bottom_right[0] - top_left[0])
                rect_height = abs(bottom_right[1] - top_left[1])

                # Ensure index finger is within the rectangle
                if top_left[0] <= ix <= bottom_right[0] and top_left[1] <= iy <= bottom_right[1]:
                    # Map cursor position to screen
                    screen_x = int((ix - top_left[0]) / rect_width * screen_width)
                    screen_y = int((iy - top_left[1]) / rect_height * screen_height)

                    # Boundary check for screen coordinates
                    screen_x = max(0, min(screen_width - 1, screen_x))
                    screen_y = max(0, min(screen_height - 1, screen_y))

                    # Move the cursor
                    pyautogui.moveTo(screen_x, screen_y)

    # Display the frame
    cv2.putText(frame, "Press 'r' to reset the rectangle.", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
    cv2.imshow("Hand Tracking", frame)

    # Key controls
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
    elif key == ord('r'):  # Reset rectangle
        top_left = None
        bottom_right = None
        rectangle_defined = False
        print("Rectangle reset.")

# Release resources
cap.release()
cv2.destroyAllWindows()
