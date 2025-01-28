import tensorflow as tf

# Define the URL of the model
MODEL_URL = 'http://download.tensorflow.org/models/object_detection/ssd_mobilenet_v2_coco_2018_03_29.tar.gz'

# Download and extract the model
model_dir = tf.keras.utils.get_file(
    'ssd_mobilenet_v2_coco_2018_03_29',
    MODEL_URL,
    untar=True,
    cache_dir='./models'
)

print(f'Model downloaded and saved to: {model_dir}')
