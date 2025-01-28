import pytesseract
import cv2
import tkinter as tk
from tkinter import messagebox
from PIL import Image, ImageTk

# If Tesseract is not in your PATH, provide the path to the Tesseract executable
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'

# Function to extract text from image
def extract_text_from_image(image):
    # Convert the image to grayscale for better OCR performance
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Apply thresholding for better clarity
    _, thresh_image = cv2.threshold(gray_image, 150, 255, cv2.THRESH_BINARY)

    # Use Tesseract to extract text from the image
    extracted_text = pytesseract.image_to_string(thresh_image)

    return extracted_text

# Function to save the extracted text
def save_text(text):
    with open("extracted_text.txt", "w") as file:
        file.write(text)
    messagebox.showinfo("Save", "Text saved successfully!")

# Initialize Tkinter window
window = tk.Tk()
window.title("Live Text Detection")

# Create a label to display webcam feed
label = tk.Label(window)
label.pack()

# Create a button to save extracted text
save_button = tk.Button(window, text="Save Extracted Text", command=lambda: save_text(extracted_text))
save_button.pack()

# Initialize webcam
cap = cv2.VideoCapture(1)
extracted_text = ""

def show_frame():
    global extracted_text
    
    # Capture frame from webcam
    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame")
        return
    
    # Extract text from the frame
    extracted_text = extract_text_from_image(frame)
    
    # Show the extracted text on the frame
    font = cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(frame, extracted_text, (50, 50), font, 1, (0, 255, 0), 2, cv2.LINE_AA)
    
    # Convert the frame to RGB (Tkinter uses RGB)
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    frame_pil = Image.fromarray(frame_rgb)
    frame_tk = ImageTk.PhotoImage(frame_pil)

    # Update the label with the frame
    label.config(image=frame_tk)
    label.image = frame_tk
    
    # Call the function again after 10ms
    window.after(10, show_frame)

# Start showing the frame
show_frame()

# Start the Tkinter main loop
window.mainloop()

# Release webcam
cap.release()
cv2.destroyAllWindows()
