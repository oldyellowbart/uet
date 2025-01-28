import cv2
import numpy as np
import pygame
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import mediapipe as mp

# Initialize OpenCV and set up video capture for both cameras
cap1 = cv2.VideoCapture(0)  # Camera 1 (left)
cap2 = cv2.VideoCapture(1)  # Camera 2 (right)

# Check if cameras opened correctly
if not cap1.isOpened() or not cap2.isOpened():
    print("Error: Could not open one or both cameras.")
    exit()

# Set up MediaPipe for face detection and landmark detection
mp_face_mesh = mp.solutions.face_mesh
face_mesh = mp_face_mesh.FaceMesh(min_detection_confidence=0.2, min_tracking_confidence=0.2)

# Create the OpenGL window
pygame.init()
pygame.display.set_mode((800, 600), pygame.DOUBLEBUF | pygame.OPENGL)

# Set up OpenGL camera
glClearColor(0.0, 0.0, 0.0, 1.0)  # Set background color to black
glEnable(GL_DEPTH_TEST)
glMatrixMode(GL_PROJECTION)
glLoadIdentity()
gluPerspective(45, (800 / 600), 0.1, 50.0)  # Set the camera view for perspective
glMatrixMode(GL_MODELVIEW)

# Function to draw a sphere (face model)
def draw_sphere(radius, slices, stacks):
    quadric = gluNewQuadric()
    gluQuadricDrawStyle(quadric, GLU_FILL)
    gluSphere(quadric, radius, slices, stacks)

# Reconstruct 3D points by combining left and right camera data
def reconstruct_3d_points(x_left, y_left, x_right, y_right, baseline, focal_length):
    # Compute disparity
    disparity = x_left - x_right
    if disparity == 0:  # Avoid division by zero
        return None

    # Calculate depth (Z) using the formula Z = (focal_length * baseline) / disparity
    Z = (focal_length * baseline) / disparity
    X = (x_left - 320) * Z / focal_length  # Assuming 320 is the center of the camera frame
    Y = (y_left - 240) * Z / focal_length  # Assuming 240 is the center of the camera frame
    return X, Y, Z

# Function to check if faces from both cameras match
def check_frame_match(faces1, faces2, threshold=50):
    if len(faces1) == len(faces2):
        for (x1, y1, w1, h1), (x2, y2, w2, h2) in zip(faces1, faces2):
            # Check if the centers of the faces are close enough
            center1 = (x1 + w1 // 2, y1 + h1 // 2)
            center2 = (x2 + w2 // 2, y2 + h2 // 2)
            distance = np.linalg.norm(np.array(center1) - np.array(center2))  # Euclidean distance
            if distance < threshold:
                return True  # Faces match
    return False  # Faces don't match

# Function to detect and draw landmarks using MediaPipe
def draw_landmarks(frame, results):
    if results.multi_face_landmarks:
        for face_landmarks in results.multi_face_landmarks:
            for landmark in face_landmarks.landmark:
                # Convert landmarks to pixel values and draw them on the frame
                h, w, _ = frame.shape
                x, y = int(landmark.x * w), int(landmark.y * h)
                cv2.circle(frame, (x, y), 1, (0, 255, 0), -1)  # Green color for landmarks
    return frame

# OpenGL rendering function
def draw_points(xyz_points):
    for point in xyz_points:
        glColor3f(0.0, 1.0, 0.0)  # Green color for the face
        glPushMatrix()
        glTranslatef(point[0], point[1], point[2])  # Translate to the 3D position
        draw_sphere(0.05, 10, 10)  # Draw a smaller sphere to represent a point
        glPopMatrix()

# Main loop
while True:
    # Handle pygame events to prevent freezing
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            cap1.release()
            cap2.release()
            cv2.destroyAllWindows()
            pygame.quit()
            exit()

    ret1, frame1 = cap1.read()  # Capture frame from Camera 1 (left)
    ret2, frame2 = cap2.read()  # Capture frame from Camera 2 (right)

    if not ret1 or not ret2:
        print("Error: Failed to capture video frames.")
        break

    # Convert frames to RGB (MediaPipe requires RGB format)
    frame1_rgb = cv2.cvtColor(frame1, cv2.COLOR_BGR2RGB)
    frame2_rgb = cv2.cvtColor(frame2, cv2.COLOR_BGR2RGB)

    # Process the frames for face detection and landmark detection
    results1 = face_mesh.process(frame1_rgb)
    results2 = face_mesh.process(frame2_rgb)

    # Draw facial landmarks on both frames
    frame1 = draw_landmarks(frame1, results1)
    frame2 = draw_landmarks(frame2, results2)

    # Lists to hold points for 3D reconstruction
    xyz_points = []

    # Camera parameters
    baseline = 100  # Horizontal distance between the two cameras (in pixels)
    focal_length = 700  # Focal length of the cameras (in pixels)

    # Process detected faces and reconstruct 3D points
    if results1.multi_face_landmarks and results2.multi_face_landmarks:
        for lm1, lm2 in zip(results1.multi_face_landmarks, results2.multi_face_landmarks):
            for landmark1, landmark2 in zip(lm1.landmark, lm2.landmark):
                # Example: Taking the nose tip landmark (landmark index 1) for reconstruction
                x1, y1 = int(landmark1.x * frame1.shape[1]), int(landmark1.y * frame1.shape[0])
                x2, y2 = int(landmark2.x * frame2.shape[1]), int(landmark2.y * frame2.shape[0])

                # Reconstruct 3D point from stereo vision
                xyz_point = reconstruct_3d_points(x1, y1, x2, y2, baseline, focal_length)
                if xyz_point:
                    xyz_points.append(xyz_point)

    # OpenGL rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()
    glTranslatef(0.0, 0.0, -5)  # Move the camera back along the Z axis

    # Rotate the points for visualization
    glRotatef(1, 3, 1, 0)  # Rotate the points in 3D space

    # Draw the 3D reconstructed points (using spheres)
    draw_points(xyz_points)

    # Show the frames in separate OpenCV windows
    cv2.imshow("Camera 1 (Left)", frame1)
    cv2.imshow("Camera 2 (Right)", frame2)

    # Update the OpenGL window to show rendered content
    pygame.display.flip()

    # Break the loop on pressing 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release resources
cap1.release()
cap2.release()
cv2.destroyAllWindows()
pygame.quit()
