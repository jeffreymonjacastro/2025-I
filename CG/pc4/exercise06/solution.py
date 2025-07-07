import cv2
import numpy as np
from ultralytics import YOLO, SAM
import time


def highlight_people_cars_and_bikes(full_path_input_image, color_scale_image, color_scale_people, color_scale_cars, color_scale_bikes, full_path_output_image):
    input_frame = cv2.imread(full_path_input_image)
    frame_height, frame_width = input_frame.shape[:2]

    detector_model = YOLO('yolov8l.pt')
    segmentation_model = SAM('sam2.1_t.pt')

    detection_results = detector_model(input_frame, conf=0.3, verbose=False)[0]

    output_canvas = np.full((frame_height, frame_width, 3), color_scale_image, dtype=np.uint8)

    if detection_results.boxes is not None:
        bounding_boxes = []
        object_classes = []

        for detected_box in detection_results.boxes:
            class_id = int(detected_box.cls)
            if class_id <= 2:
                coord_x1, coord_y1, coord_x2, coord_y2 = detected_box.xyxy[0].tolist()
                bounding_boxes.append([coord_x1, coord_y1, coord_x2, coord_y2])
                object_classes.append(class_id)

        if bounding_boxes:
            segmentation_results = segmentation_model(input_frame, bboxes=bounding_boxes, verbose=False)

            color_mapping = [color_scale_people, color_scale_bikes, color_scale_cars]

            if len(segmentation_results) > 0 and segmentation_results[0].masks is not None:
                extracted_masks = segmentation_results[0].masks.data
                for mask_index in range(len(bounding_boxes)):
                    if mask_index < len(extracted_masks):
                        current_mask = extracted_masks[mask_index].cpu().numpy()
                        output_canvas[current_mask > 0.5] = color_mapping[object_classes[mask_index]]

    cv2.imwrite(full_path_output_image, output_canvas)


highlight_people_cars_and_bikes(
    full_path_input_image='ejemplo1.png',
    color_scale_image=(255, 255, 255),
    color_scale_people=(0, 255, 0),
    color_scale_cars=(0, 0, 255),
    color_scale_bikes=(255, 0, 0),
    full_path_output_image='output.png'
)