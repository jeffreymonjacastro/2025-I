import cv2
import numpy as np
from enum import Enum

Blender = Enum('Blender', 'FeatherBlender MultiBandBlender')
FeaturesFinder = Enum('FeaturesFinder', 'AKAZE ORB SIFT SURF')
FeaturesMatcher = Enum('FeaturesMatcher', 'AffineBestOf2Nearest BestOf2NearestRange')
Warper = Enum('Warper', 'Affine CompressedRectilinearPortrait CompressedRectilinear Cylindrical Fisheye Mercator PaniniPortrait Panini Plane Spherical Stereographic TransverseMercator')

def stitch_images(
    full_path_input_image: list[str], 
    blender: Blender, 
    features_finder: FeaturesFinder, 
    features_matcher: FeaturesMatcher, 
    warper: Warper, 
    full_path_output_image: str
):
    imgs = []
    for i, filename in enumerate(full_path_input_image):
        img = cv2.imread(filename)
        if img is not None:
            imgs.append(img)

    if len(imgs) < 2:
        print("Not enough images to stitch.")
        return False
    
    detector = cv2.SIFT.create()

    for i, img in enumerate(imgs):
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        kp, desc = detector.detectAndCompute(gray, None)
        print(f"Imagen {i+1}: {len(kp)} keypoints detectados")
    
    try:
        stitcher = cv2.Stitcher.create(cv2.Stitcher_PANORAMA)
        
        stitcher.setRegistrationResol(0.5)  
        stitcher.setSeamEstimationResol(0.1)  
        stitcher.setCompositingResol(1.0)  
        stitcher.setPanoConfidenceThresh(0.7)  
        
        if warper == Warper.Cylindrical:
            stitcher.setWarper(cv2.CylindricalWarper())
            print("Usando warper cilíndrico...")
        elif warper == Warper.Spherical:
            stitcher.setWarper(cv2.SphericalWarper())
            print("Usando warper esférico...")
        else: 
            print("Usando warper plano...")
        
        print("\nIniciando el proceso de stitching...")
        
        status, panorama = stitcher.stitch(imgs)
        
        if status == cv2.Stitcher_OK:
            print("✓ Stitching exitoso!")
            panorama_cropped = crop_black_borders(panorama)
            
            cv2.imwrite(full_path_output_image, panorama_cropped)
            print(f"✓ Panorama guardado en: {full_path_output_image}")
            print(f"Dimensiones finales: {panorama_cropped.shape}")
            return True
        else:
            error_messages = {
                cv2.Stitcher_ERR_NEED_MORE_IMGS: "Se necesitan más imágenes",
                cv2.Stitcher_ERR_HOMOGRAPHY_EST_FAIL: "Fallo en estimación de homografía",
                cv2.Stitcher_ERR_CAMERA_PARAMS_ADJUST_FAIL: "Fallo en ajuste de parámetros de cámara"
            }
            print(f"✗ Error en el stitching: {error_messages.get(status, f'Código {status}')}")
            
            print("\nIntentando con diferentes configuraciones...")
            return stitch_fallback(imgs, full_path_output_image)
            
    except Exception as e:
        print(f"✗ Error durante el stitching: {e}")
        print("Intentando método alternativo...")
        return stitch_fallback(imgs, full_path_output_image)

def crop_black_borders(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    _, thresh = cv2.threshold(gray, 1, 255, cv2.THRESH_BINARY)
    
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    if contours:
        largest_contour = max(contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(largest_contour)
        
        cropped = image[y:y+h, x:x+w]
        return cropped
    
    return image

def stitch_fallback(imgs, output_path):
    try:
        modes = [cv2.Stitcher_PANORAMA, cv2.Stitcher_SCANS]
        mode_names = ["PANORAMA", "SCANS"]
        
        for mode, mode_name in zip(modes, mode_names):
            print(f"\n--- Probando modo {mode_name} ---")
            stitcher = cv2.Stitcher.create(mode)
            
            stitcher.setRegistrationResol(0.6)
            stitcher.setSeamEstimationResol(0.1) 
            stitcher.setCompositingResol(1.0)
            stitcher.setPanoConfidenceThresh(0.8)  
            
            orders = [
                [0, 1, 2, 3],  
                [3, 2, 1, 0],  
                [1, 0, 2, 3],  
                [0, 2, 1, 3],  
                [2, 1, 0, 3],  
                [0, 1, 3, 2],  
                [1, 2, 0, 3],  
                [1, 2, 3, 0],  
            ]
            
            for order in orders:
                try:
                    reordered_imgs = [imgs[j] for j in order]
                    print(f"  Probando orden: {[i+1 for i in order]}")
                    
                    status, panorama = stitcher.stitch(reordered_imgs)
                    
                    if status == cv2.Stitcher_OK:
                        panorama_cropped = crop_black_borders(panorama)
                        cv2.imwrite(output_path, panorama_cropped)
                        print(f"  ✓ ¡ÉXITO! Modo {mode_name}, orden {[i+1 for i in order]}")
                        print(f"  ✓ Guardado en: {output_path}")
                        return True
                    else:
                        print(f"  ✗ Falló con código {status}")
                        
                except Exception as e:
                    print(f"  ✗ Error: {e}")
                    continue
        
        print(f"\n--- Probando con resolución reducida ---")
        resized_imgs = []
        for img in imgs:
            height, width = img.shape[:2]
            if width > 800:  
                scale = 800.0 / width
                new_width = int(width * scale)
                new_height = int(height * scale)
                resized_img = cv2.resize(img, (new_width, new_height))
                resized_imgs.append(resized_img)
            else:
                resized_imgs.append(img)
        
        warpers = [
            (cv2.PlaneWarper(), "Plane"),
            (cv2.CylindricalWarper(), "Cylindrical"),
            (cv2.SphericalWarper(), "Spherical"),
        ]
        
        for warper, warper_name in warpers:
            print(f"  Probando warper {warper_name}...")
            stitcher = cv2.Stitcher.create(cv2.Stitcher_PANORAMA)
            stitcher.setWarp(warper)
            stitcher.setRegistrationResol(0.5)
            stitcher.setSeamEstimationResol(0.1)
            
            for order in [[0,1,2,3], [3,2,1,0], [0,1,3,2], [1,0,2,3]]:
                try:
                    reordered_imgs = [resized_imgs[j] for j in order]
                    status, panorama = stitcher.stitch(reordered_imgs)
                    
                    if status == cv2.Stitcher_OK:
                        panorama_cropped = crop_black_borders(panorama)
                        cv2.imwrite(output_path, panorama_cropped)
                        print(f"  ✓ ¡ÉXITO! Warper {warper_name}, orden {[i+1 for i in order]}")
                        return True
                        
                except Exception:
                    continue
        
        print(f"\n--- Probando sin imagen 4 (perspectiva diferente) ---")
        try:
            subset_imgs = [imgs[0], imgs[1], imgs[2]] 
            stitcher = cv2.Stitcher.create(cv2.Stitcher_PANORAMA)
            stitcher.setRegistrationResol(0.6)
            stitcher.setSeamEstimationResol(0.1)
            
            status, partial_panorama = stitcher.stitch(subset_imgs)
            
            if status == cv2.Stitcher_OK:
                print("  ✓ Stitching parcial exitoso (imágenes 1,2,3)")
                try:
                    final_imgs = [partial_panorama, imgs[3]]  
                    status2, final_panorama = stitcher.stitch(final_imgs)
                    
                    if status2 == cv2.Stitcher_OK:
                        panorama_cropped = crop_black_borders(final_panorama)
                        cv2.imwrite(output_path, panorama_cropped)
                        print("  ✓ ¡ÉXITO! Agregada imagen 4 al panorama parcial")
                        return True
                    else:
                        panorama_cropped = crop_black_borders(partial_panorama)
                        cv2.imwrite(output_path, panorama_cropped)
                        print("  ⚠ Guardado panorama parcial (imágenes 1,2,3 sin imagen 4)")
                        return True
                except Exception:
                    # Guardar panorama parcial como último recurso
                    panorama_cropped = crop_black_borders(partial_panorama)
                    cv2.imwrite(output_path, panorama_cropped)
                    print("  ⚠ Guardado panorama parcial (imágenes 1,2,3 sin imagen 4)")
                    return True
        except Exception:
            pass
        
        return False
            
    except Exception as e:
        print(f"✗ Error en método alternativo: {e}")
        return False

success = stitch_images(
    full_path_input_image=[
        'panoramas/singapur/1.png',
        'panoramas/singapur/2.png', 
        'panoramas/singapur/3.png',
        'panoramas/singapur/4.png'
    ],
    blender=Blender.FeatherBlender,
    features_finder=FeaturesFinder.SIFT,
    features_matcher=FeaturesMatcher.BestOf2NearestRange,
    warper=Warper.Cylindrical,  
    full_path_output_image='panorama1.jpg'
)