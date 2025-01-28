import cv2
import mediapipe as mp

# Initialize MediaPipe FaceMesh
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(
    min_detection_confidence=0.5, 
    min_tracking_confidence=0.5
)

# Start video capture
cap = cv2.VideoCapture(1)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    
    frame = cv2.flip(frame, 1)

    try:
        # Convert the frame to RGB
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = face_mesh.process(image)

        # Convert back to BGR for OpenCV
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        # Draw face landmarks if they exist
        if results.multi_face_landmarks:
            for face_landmarks in results.multi_face_landmarks:
                mp.solutions.drawing_utils.draw_landmarks(
                    image, face_landmarks, mp_face_mesh.FACEMESH_CONTOURS)
        
        else:
            print("No face landmarks detected")

    except Exception as e:
        print(f"Error processing frame: {e}")

    # Display the frame
    cv2.imshow('Face Mesh', image)

    # Exit the loop when 'q' is pressed
    if cv2.waitKey(5) & 0xFF == ord('q'):
        break

# Release the capture and close windows
cap.release()
cv2.destroyAllWindows()
