import tensorflow as tf
import numpy as np
import cv2
import os

# Function to ensure memory growth
def set_memory_growth():
    physical_devices = tf.config.list_physical_devices('GPU')
    for device in physical_devices:
        try:
            tf.config.experimental.set_memory_growth(device, True)
        except:
            pass  # Handle error if memory growth can't be set

# Set up memory growth to avoid allocation of all GPU memory
set_memory_growth()

# Try using GPU, otherwise fall back to CPU
try:
    # Load model
    model = tf.saved_model.load(r"C:\Users\dell\Downloads\ssd_mobilenet_v2_coco_2018_03_29\ssd_mobilenet_v2_coco_2018_03_29\saved_model")
    print("Model loaded successfully on GPU.")
except tf.errors.ResourceExhaustedError as e:
    print("Out of memory error, switching to CPU...")
    with tf.device('/CPU:0'):
        model = tf.saved_model.load(r"C:\Users\dell\Downloads\ssd_mobilenet_v2_coco_2018_03_29\ssd_mobilenet_v2_coco_2018_03_29\saved_model")
        print("Model loaded successfully on CPU.")

# Use the detection function signature from the loaded model
infer = model.signatures['serving_default']

# Function to process each webcam frame
def process_frame(frame):
    # Convert image from BGR to RGB
    image_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    # Resize to fit the model input size
    image_resized = cv2.resize(image_rgb, (300, 300))  # Resize to match the model input
    # Normalize and add batch dimension, but keep uint8 type
    image_resized_uint8 = np.uint8(image_resized)  # Ensure the image is uint8
    image_normalized = np.expand_dims(image_resized_uint8, axis=0)
    return tf.convert_to_tensor(image_normalized, dtype=tf.uint8)  # Pass uint8 tensor

# Function to detect objects in live webcam feed
def detect_objects_in_video():
    # Open the webcam (0 for the default webcam)
    cap = cv2.VideoCapture(1)
    
    if not cap.isOpened():
        print("Error: Could not open webcam.")
        return
    
    while True:
        ret, frame = cap.read()  # Read a frame from the webcam
        if not ret:
            print("Error: Failed to capture frame.")
            break
        
        # Process the frame before passing to the model
        input_tensor = process_frame(frame)
        
        # Run inference (pass the frame to the model using the 'serving_default' signature)
        detections = infer(input_tensor)
        
        # Process the output of the model (you can extract the boxes and class ids)
        boxes = detections['detection_boxes'][0].numpy()  # Extract bounding box coordinates
        class_ids = detections['detection_classes'][0].numpy()  # Extract class IDs
        scores = detections['detection_scores'][0].numpy()  # Extract scores
        
        # Draw bounding boxes on the frame for each detected object
        for i in range(len(boxes)):
            if scores[i] > 0.5:  # Threshold score for detection (e.g., only consider score > 0.5)
                ymin, xmin, ymax, xmax = boxes[i]
                h, w, _ = frame.shape
                start_point = (int(xmin * w), int(ymin * h))
                end_point = (int(xmax * w), int(ymax * h))
                cv2.rectangle(frame, start_point, end_point, (0, 255, 0), 2)
        
        # Display the frame with the detected objects
        cv2.imshow('Live Stream Object Detection', frame)
        
        # Exit the loop when 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    cv2.destroyAllWindows()

# Run the live webcam object detection
detect_objects_in_video()
