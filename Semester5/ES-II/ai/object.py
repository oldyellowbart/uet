import cv2 as cv
from ultralytics import YOLO

model = YOLO('yolo11n.pt')

while True:
    results = model(source=0, show=True, conf=0.6, save=True)
    if cv.waitKey(1) & 0xFF == ord('q'):
        break