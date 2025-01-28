import cv2
import mediapipe as mp
import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *
import pygame
from pygame.locals import *

# Initialize MediaPipe FaceMesh
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.5, min_tracking_confidence=0.5)

# Start video capture
cap = cv2.VideoCapture(0)

# Function to create a 3D face using landmark points
def draw_3d_face(landmarks):
    glBegin(GL_POINTS)
    for point in landmarks:
        glVertex3f(point[0], point[1], point[2])  # X, Y, Z coordinates
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
        # Simple conversion: scale the x and y, leave z as zero for simplicity
        x = landmark.x * 2  # Scale factor
        y = landmark.y * 2  # Scale factor
        z = landmark.z * 2  # Scale depth based on Z (3D coordinate)

        # Horizontal flip: negate x for horizontal mirroring
        x = -x
        
        # Vertical flip: negate y for vertical mirroring
        y = -y

        face_3d.append([x, y, z])
    return face_3d

# OpenGL rendering function
def render_frame(frame):
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    
    # Convert the frame to RGB for MediaPipe processing
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = face_mesh.process(image)
    
    # Check if no face is detected
    if results.multi_face_landmarks:
        # If faces are detected, process each one
        for face_landmarks in results.multi_face_landmarks:
            landmarks_3d = convert_to_3d(face_landmarks.landmark)
            draw_3d_face(landmarks_3d)
    else:
        print("No face detected")

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

        render_frame(frame)
        
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
