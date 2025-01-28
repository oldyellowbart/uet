import cv2
import mediapipe as mp
import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *
import pygame
from pygame.locals import *

# Initialize MediaPipe solutions for Face, Hands, and Pose
mp_face_mesh = mp.solutions.face_mesh
mp_hands = mp.solutions.hands
mp_pose = mp.solutions.pose

face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.5, min_tracking_confidence=0.5)
hands = mp_hands.Hands(min_detection_confidence=0.5, min_tracking_confidence=0.5)
pose = mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5)

# Start video capture
cap = cv2.VideoCapture(0)

# Function to create a 3D face using landmark points and drawing mesh
def draw_3d_face(landmarks, connections=None):
    glBegin(GL_POINTS)
    for point in landmarks:
        glVertex3f(point[0], point[1], point[2])  # X, Y, Z coordinates
    glEnd()

    if connections:
        glBegin(GL_LINES)
        for start, end in connections:
            glVertex3f(landmarks[start][0], landmarks[start][1], landmarks[start][2])
            glVertex3f(landmarks[end][0], landmarks[end][1], landmarks[end][2])
        glEnd()

# Function to set up the OpenGL environment
def setup_3d_view():
    glClearColor(0, 0, 0, 1)  # Set background color to black
    glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45, 1, 0.1, 50.0)  # Set perspective view
    glTranslatef(0.0, 0.0, -5)  # Move the face a bit away from camera

# Convert 2D landmarks to 3D (this is a simple example, can be improved)
def convert_to_3d(landmarks):
    face_3d = []
    for landmark in landmarks:
        x = landmark.x * 2  # Scale factor
        y = landmark.y * 2  # Scale factor
        z = landmark.z * 2  # Scale depth based on Z (3D coordinate)

        # Horizontal flip: negate x for horizontal mirroring
        x = -x
        
        # Vertical flip: negate y for vertical mirroring
        y = -y

        face_3d.append([x, y, z])
    return face_3d

# Convert hand landmarks to 3D
def convert_hand_to_3d(landmarks):
    hand_3d = []
    for landmark in landmarks:
        x = landmark.x * 2  # Scale factor
        y = landmark.y * 2  # Scale factor
        z = landmark.z * 2  # Scale depth based on Z (3D coordinate)

        # Horizontal flip
        x = -x
        # Vertical flip
        y = -y

        hand_3d.append([x, y, z])
    return hand_3d

# Convert body landmarks to 3D
def convert_pose_to_3d(landmarks):
    pose_3d = []
    for landmark in landmarks:
        x = landmark.x * 2  # Scale factor
        y = landmark.y * 2  # Scale factor
        z = landmark.z * 2  # Scale depth based on Z (3D coordinate)

        # Horizontal flip
        x = -x
        # Vertical flip
        y = -y

        pose_3d.append([x, y, z])
    return pose_3d

# Function to overlay the 2D landmarks on the frame (for physical points)
def overlay_landmarks_on_frame(frame, landmarks, color=(0, 255, 0), radius=5):
    for landmark in landmarks:
        x = int(landmark.x * frame.shape[1])
        y = int(landmark.y * frame.shape[0])
        cv2.circle(frame, (x, y), radius, color, -1)  # Draw circle at landmark

# OpenGL rendering function
def render_frame(frame):
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    
    # Convert the frame to RGB for MediaPipe processing
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # Process face landmarks
    face_results = face_mesh.process(image)
    if face_results.multi_face_landmarks:
        for face_landmarks in face_results.multi_face_landmarks:
            landmarks_3d = convert_to_3d(face_landmarks.landmark)
            draw_3d_face(landmarks_3d, mp_face_mesh.FACEMESH_TESSELATION)  # Draw face mesh using predefined connections
    
    # Process hand landmarks
    hand_results = hands.process(image)
    if hand_results.multi_hand_landmarks:
        for hand_landmarks in hand_results.multi_hand_landmarks:
            hand_3d = convert_hand_to_3d(hand_landmarks.landmark)
            draw_3d_face(hand_3d)  # Draw hand mesh (if needed, you can define hand connections)

    # Process body (pose) landmarks
    pose_results = pose.process(image)
    if pose_results.pose_landmarks:
        pose_3d = convert_pose_to_3d(pose_results.pose_landmarks.landmark)
        draw_3d_face(pose_3d)  # Draw body mesh (can define connections for body parts)

    pygame.display.flip()

# Initialize PyGame window
def main():
    pygame.init()
    screen = pygame.display.set_mode((800, 600), DOUBLEBUF | OPENGL)
    setup_3d_view()  # Set up the 3D view for OpenGL
    
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Process and overlay 2D landmarks on the frame
        image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        face_results = face_mesh.process(image)
        if face_results.multi_face_landmarks:
            for face_landmarks in face_results.multi_face_landmarks:
                overlay_landmarks_on_frame(frame, face_landmarks.landmark)
        
        hand_results = hands.process(image)
        if hand_results.multi_hand_landmarks:
            for hand_landmarks in hand_results.multi_hand_landmarks:
                overlay_landmarks_on_frame(frame, hand_landmarks.landmark)
        
        pose_results = pose.process(image)
        if pose_results.pose_landmarks:
            overlay_landmarks_on_frame(frame, pose_results.pose_landmarks.landmark)
        
        # Display the 2D frame with the physical points overlaid
        cv2.imshow('Live Video Stream with Landmarks', frame)

        render_frame(frame)  # Render the 3D model

        # Handle PyGame events
        for event in pygame.event.get():
            if event.type == QUIT:
                cap.release()
                pygame.quit()
                return
            elif event.type == KEYDOWN:
                # Exit the loop if ESC key is pressed
                if event.key == K_ESCAPE:
                    cap.release()
                    pygame.quit()
                    return
        
        # Exit the loop when 'q' is pressed (for OpenCV window)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    pygame.quit()

# Run the main function
if __name__ == "__main__":
    main()
