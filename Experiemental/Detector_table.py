import cv2
import numpy as np
import pyttsx3
import tensorflow as tf
from tensorflow import keras
import time

# Limit TensorFlow to only use as much GPU memory as needed
physical_devices = tf.config.list_physical_devices('GPU')
if physical_devices:
    tf.config.experimental.set_memory_growth(physical_devices[0], True)

# Load pre-trained object detection model (e.g., MobileNet SSD or COCO dataset)
model = tf.saved_model.load(r"C:\Users\dell\Desktop\2022_MC_58\Experiemental\Models\Video_Codec_SDK_12.2.72\saved_model")

# Initialize Text-to-Speech engine
engine = pyttsx3.init()

# Initialize video captures
cap = cv2.VideoCapture(0)

# Define a function to get object name
def get_object_name(class_id):
    labels = [
        'background', 'person', 'bicycle', 'car', 'motorbike', 'airplane', 'bus', 'train', 'truck', 'boat',
        'traffic light', 'fire hydrant', 'none', 'stop sign', 'parking meter', 'bench', 'bird', 'cat', 'dog',
        'horse', 'sheep', 'cow', 'elephant', 'bear', 'zebra', 'giraffe', 'none', 'backpack', 'umbrella', 'none',
        'handbag', 'tie', 'suitcase', 'frisbee', 'skis', 'snowboard', 'sports ball', 'kite', 'baseball bat', 'baseball glove',
        'skateboard', 'surfboard', 'tennis racket', 'bottle', 'wine glass', 'cup', 'fork', 'knife', 'spoon', 'bowl',
        'banana', 'apple', 'sandwich', 'orange', 'broccoli', 'carrot', 'hot dog', 'pizza', 'donut', 'cake',
        'chair', 'couch', 'potted plant', 'bed', 'dining table', 'toilet', 'tv', 'laptop', 'mouse', 'remote', 'keyboard',
        'cell phone', 'microwave', 'oven', 'toaster', 'sink', 'refrigerator', 'book', 'clock', 'vase', 'scissors',
        'teddy bear', 'hair drier', 'toothbrush'
    ]
    return labels[class_id]

# Function to perform detection
def detect_objects(frame):
    # Preprocess the image for object detection
    input_tensor = tf.convert_to_tensor(frame)
    input_tensor = input_tensor[tf.newaxis,...]
    
    # Perform detection
    detections = model(input_tensor)

    # Get detected boxes, classes, and scores
    boxes = detections['detection_boxes'][0].numpy()
    classes = detections['detection_classes'][0].numpy().astype(np.int32)
    scores = detections['detection_scores'][0].numpy()

    return boxes, classes, scores

# Function for text-to-speech
def speak(text):
    engine.say(text)
    engine.runAndWait()

# Main loop
def main():
    object_positions = {}  # Dictionary to store object names and positions
    
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break
        
        # Detect objects in the frame
        boxes, classes, scores = detect_objects(frame)

        for i in range(len(boxes)):
            if scores[i] > 0.5:  # Only consider objects with high confidence
                box = boxes[i]
                class_id = classes[i] - 1  # class_id starts from 1, but our label list starts from 0
                object_name = get_object_name(class_id)
                
                # Calculate coordinates (you can use box to track position)
                ymin, xmin, ymax, xmax = box
                (left, right, top, bottom) = (xmin * frame.shape[1], xmax * frame.shape[1], ymin * frame.shape[0], ymax * frame.shape[0])

                # Draw bounding box and label on frame
                cv2.rectangle(frame, (int(left), int(top)), (int(right), int(bottom)), (0, 255, 0), 2)
                cv2.putText(frame, object_name, (int(left), int(top) - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

                # Store object position (in real-time, if you need)
                object_positions[object_name] = (int(left), int(top), int(right), int(bottom))

                # Speak the object detected
                speak(f"Detected {object_name} at position ({int(left)}, {int(top)})")

        # Display the resulting frame
        cv2.imshow('Live Stream - Object Detection', frame)

        # Exit condition (Esc key)
        if cv2.waitKey(1) & 0xFF == 27:
            break

    cap.release()
    cv2.destroyAllWindows()

# Run the program
if __name__ == "__main__":
    main()
