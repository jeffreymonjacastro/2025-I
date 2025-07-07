from ultralytics import YOLO
from time import time


def count_people_cars_and_bikes(full_path_input_vide: str) -> list:
    detector_model = YOLO("yolov8l.pt")

    object_counters = {0: set(), 1: set(), 2: set()}

    tracking_results = detector_model.track(
        full_path_input_vide,
        persist=True,
        tracker="bytetrack.yaml",
        verbose=False,
        stream=True,
    )

    for detection_result in tracking_results:
        if detection_result.boxes is not None and detection_result.boxes.id is not None:
            object_ids = detection_result.boxes.id.cpu().numpy().astype(int)
            object_classes = detection_result.boxes.cls.cpu().numpy().astype(int)
            confidence_scores = detection_result.boxes.conf.cpu().numpy()

            for tracked_id, object_class, confidence in zip(object_ids, object_classes, confidence_scores):
                if object_class in [0, 1, 2] and confidence > 0.5:
                    object_counters[object_class].add(tracked_id)

    people_count = len(object_counters[0])
    cars_count = len(object_counters[1])
    bikes_count = len(object_counters[2])

    return [people_count, cars_count, bikes_count]


if __name__ == "__main__":
    full_path_input_vide = "video.mp4"

    start_time = time()
    people, cars, bikes = count_people_cars_and_bikes(full_path_input_vide)
    end_time = time()

    print(f"Processing time: {end_time - start_time:.2f} seconds")

    print(f"People: {people}, Cars: {cars}, Bikes: {bikes}")