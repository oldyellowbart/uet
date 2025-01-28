import cv2 as cv
import mediapipe as mp
mp_pose = mp.solutions.pose
mp_drawing = mp.solutions.drawing_utils 
mp_drawing_styles = mp.solutions.drawing_styles
video = cv.VideoCapture(0)
video.set(3,500)
video.set(4,500)

while(True):
    ret, frame = video.read()
    frame = cv.flip(frame, 1) #Flip the frame 
    #pose = mp_pose.Pose()
    # 'with' statement is used for file handling, it ensures that files are automatically closed after use, preventing resource leaks
    with mp_pose.Pose(static_image_mode=True, min_detection_confidence=0.5, model_complexity=2) as pose:
        results = pose.process(frame)
        #results.pose_landmarks.landmark
        mp_drawing.draw_landmarks(
            frame,
            results.pose_landmarks,
            mp_pose.POSE_CONNECTIONS, 
            landmark_drawing_spec=mp_drawing_styles.get_default_pose_landmarks_style())

    cv.imshow('Live video', frame)
    
    if cv.waitKey(1) & 0xFF == ord('q'):
        break

video.release()
cv.destroyAllWindows()