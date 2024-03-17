import cv2
import os
import face_recognition
import pickle
import shutil
import time
import subprocess

# SHAPE_PREDICTOR_PATH = 'shape_predictor_68_face_landmarks.dat'
password_file_path = r'default_password.txt'
dataset_path = r'C:\Python\dataset'
master_folder = r"C:\Python\dataset\Master"
normal_user_folder = r"C:\Python\dataset\Normal Users"
known_master_encodings = {}
known_face_encodings_dict = {}
known_encodings ={}

global default_password 
try:
    with open(password_file_path, 'r') as file:
        default_password = str(file.read())
        
except FileNotFoundError:
        # File doesn't exist, which likely means the variable hasn't been saved previously.
        print("No saved value found. Using default.")
        default_password = "1234"

def create_folders_if_not_exist(*folders):
    
    for folder in folders:
        if not os.path.exists(folder):
            os.makedirs(folder)


def load_images_and_encode_faces(user_folder_path, person_name):
    encoding_file = os.path.join(user_folder_path, f'{person_name}_encodings.pkl')

    if os.path.exists(encoding_file):
        with open(encoding_file, 'rb') as file:
            known_face_encodings = pickle.load(file)
        print(f"Face encodings loaded for {person_name} from file.")
    else:
        known_face_encodings = []

        print(f"Loading and encoding faces for {person_name} from folder {user_folder_path}")

        image_files = [f for f in os.listdir(user_folder_path) if f.endswith(('.jpg', '.jpeg', '.png'))]

        for image_file in image_files:
            image_path = os.path.join(user_folder_path, image_file)
            print(f"Processing image: {image_path}")

            image = face_recognition.load_image_file(image_path)
            face_encodings = face_recognition.face_encodings(image)
            if face_encodings:
                encoding = face_encodings[0]  # Assuming each image contains only one face
                known_face_encodings.append(encoding)
            else:
                print(f"No face detected in {image_path}")
                # Delete the image file with no detected face
                os.remove(image_path)
                print(f"Deleted {image_path}")

            time.sleep(1.5) 

        if known_face_encodings:
            with open(encoding_file, 'wb') as file:
                pickle.dump(known_face_encodings, file)

            print(f"Face encodings loaded and saved successfully for {person_name}.")
        else:
            print(f"No face detected in any images for {person_name}. No encodings saved.")

    return known_face_encodings


def recognize_faces(known_face_encodings_dict):
    print("Starting face recognition")

    cap = cv2.VideoCapture(0)
    cap.set(3, 680*2)
    cap.set(4, 480*2)

    while True:
        ret, frame = cap.read()

        face_locations = face_recognition.face_locations(frame)
        face_encodings = face_recognition.face_encodings(frame, face_locations)

        recognized = False  

        for face_encoding in face_encodings:
            for person_name, known_encodings in known_face_encodings_dict.items():
                matches = face_recognition.compare_faces(known_encodings, face_encoding)
                if True in matches:
                    print(f"Welcome {person_name}")
                    recognized = True
                    return person_name

            if recognized:
                break  

        if not recognized:
            print("Failed to recognize any authorized user.")
            return

        cv2.imshow("Frame", frame)

        esc = cv2.waitKey(1)
        if esc == 27:
            break

    cap.release()
    cv2.destroyAllWindows()
    print("Face recognition ended")



def display_users(dataset_path,master_folder, normal_user_folder):
    master_user_encodings = load_master_user_encodings(master_folder)
    if master_user_encodings:
        known_face_encodings_dict["Master"] = master_user_encodings
    else:

        print("there is no master please register first.")
        return
    master_folder = os.path.join(dataset_path, "Master")
    normal_user_folder = os.path.join(dataset_path, "Normal Users")

    print("Master Users:")
    master_users = os.listdir(master_folder)
    print(master_users)

    print("Normal Users:")
    normal_users = os.listdir(normal_user_folder)
    print(normal_users)



def join_user(known_face_encodings_dict, master_folder, normal_user_folder):
    # Load master user encodings
    master_user_encodings = load_master_user_encodings(master_folder)
    if master_user_encodings:
        known_face_encodings_dict["Master"] = master_user_encodings
    else:
        register_user(dataset_path, known_face_encodings_dict, master_folder, normal_user_folder)
        return
    
    # Load normal user encodings
    normal_users_encodings = {}
    user_folders = [f.path for f in os.scandir(normal_user_folder) if f.is_dir()]

    for user_folder in user_folders:
        person_name = os.path.basename(user_folder)
        user_encodings = load_images_and_encode_faces(user_folder, person_name)
        normal_users_encodings[person_name] = user_encodings

    # Merge master and normal user encodings into known_face_encodings_dict
    known_face_encodings_dict.update(normal_users_encodings)

    recognized_person = recognize_faces(known_face_encodings_dict)
    if recognized_person:
        print(f"Recognized person: {recognized_person}")
        if recognized_person in known_face_encodings_dict:
            print("Starting the second code...")
            subprocess.run(["python", "Test.py"], check=True)





def load_master_user_encodings(master_folder):
    master_user_encodings_files = []

    # Check if master folder exists
    if os.path.exists(master_folder) and os.path.isdir(master_folder):
        for root, dirs, files in os.walk(master_folder):
            for file in files:
                if file.endswith('_encodings.pkl'):
                    master_user_encodings_files.append(os.path.join(root, file))

        if master_user_encodings_files:
            # Assuming we have only one encoding file for simplicity, 
            master_user_encodings_file = master_user_encodings_files[0]

            with open(master_user_encodings_file, 'rb') as file:
                master_user_encodings = pickle.load(file)
            # print("Master user encodings loaded.")
            return master_user_encodings
        else:
            # print("No master user encodings found.")
            return None
    else:
        print("Master folder does not exist.")
        return None
    
def register_user(dataset_path, known_face_encodings_dict, master_folder, normal_user_folder):

        create_folders_if_not_exist(dataset_path, master_folder, normal_user_folder)
        master_user_encodings = load_master_user_encodings(master_folder)
        if master_user_encodings:
            known_face_encodings_dict["Master"] = master_user_encodings
        else:

            password_attempt = input("No master user found. Please enter the password to register: ")
            if password_attempt != default_password:
                print("Incorrect password. Access denied.")
                return

        # Load master user encodings if master exists
        if master_user_encodings:
            known_master_encodings = load_master_user_encodings(master_folder)

            # Open the camera to recognize the user immediately
            cap = cv2.VideoCapture(0)
            cap.set(3, 680 * 2)
            cap.set(4, 480 * 2)

            recognized_user = False

            while not recognized_user:
                ret, frame = cap.read()

                # Recognize user using face encodings
                face_encodings = face_recognition.face_encodings(frame)
                for face_encoding in face_encodings:
                    matches = face_recognition.compare_faces(known_master_encodings, face_encoding)
                    if True in matches:
                        recognized_user = True
                        print("Master user recognized.")
                        break

                if recognized_user:
                    break

                cv2.imshow("User Recognition", frame)
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break

                # Check if there are faces other than the master
                face_locations = face_recognition.face_locations(frame)
                if len(face_locations) > 0:
                    print("Unauthorized user detected. Please enter the password.")
                    break

            cap.release()
            cv2.destroyAllWindows()

            if not recognized_user:
                # If the user is not recognized, ask for the password
                password_attempt = input("Enter the password: ")
                if password_attempt != default_password:
                    print("Incorrect password. Access denied.")
                    return
                

        if not master_user_encodings:
            name = input("Enter the name of the Master: ")
            master_folder_path = os.path.join(master_folder, name)

            os.makedirs(master_folder_path, exist_ok=True)

            print(f"Please look at the camera.")

            cap = cv2.VideoCapture(0)
            cap.set(3, 680 * 2)
            cap.set(4, 480 * 2)

            image_count = 0
            while image_count < 10:
                ret, frame = cap.read()

                face_locations = face_recognition.face_locations(frame)

                if face_locations:
                    image_path = os.path.join(master_folder_path, f"{name}_{image_count}.jpg")
                    cv2.imwrite(image_path, frame)
                    image_count += 1
                    if image_count == 1:
                        print(f"We took {image_count} Picture ")
                    else:
                        print(f"We took {image_count} Pictures ")

                elif not face_locations:
                    i=6
                    while i>=0:
                        print(f'{i} seconds remaining...')
                        time.sleep(1)
                        i-=1
                        ret, frame = cap.read()
                        face_locations = face_recognition.face_locations(frame)
                        if face_locations:
                            break
                        elif i==0:
                            print('Time is up..please register again')
                            time.sleep(1)
                            shutil.rmtree(master_folder_path)
                            return

                    
                cv2.imshow("Capture Images", frame)

                if cv2.waitKey(1) & 0xFF == ord('a'):
                    break

            cap.release()
            cv2.destroyAllWindows()

            known_face_encodings = load_images_and_encode_faces(master_folder_path, name)
            known_face_encodings_dict[name] = known_face_encodings
            tries = 3

            known_face_encodings_dict["Master"] = master_user_encodings
            print(f"User {name} has been registered successfully.")

            for passsword in range(tries):
                new_password = input('Enter the new password (3-10 characters): ')
                if len(new_password) in range(3,11) and new_password.isalnum():
                    temp = new_password
                    new_password = input('Confirm the password: ')
                    if temp == new_password:
                        with open(password_file_path, 'w') as file:
                            file.write(str(new_password))
                        print('Password changed successfully')
                        break
                    else: 
                        tries-=1
                        print('Confirmation failed..')
                else:
                    tries-=1
                    if tries!=0:
                        print(f'Try again..{tries} tries left')
                    elif tries == 0:
                        print('No tries left, the password remains the same.')

            
                    

        else:

            name = input("Enter the name of the new user: ")
            user_folder_path = os.path.join(normal_user_folder, name)
            os.makedirs(user_folder_path, exist_ok=True)

            print(f"Please look at the camera.")

            cap = cv2.VideoCapture(0)
            cap.set(3, 680 * 2)
            cap.set(4, 480 * 2)

            image_count = 0
            while image_count < 10:
                ret, frame = cap.read()

                cv2.imshow("Capture Images", frame)

                face_locations = face_recognition.face_locations(frame)

                if face_locations:
                    image_path = os.path.join(user_folder_path, f"{name}_{image_count}.jpg")
                    cv2.imwrite(image_path, frame)
                    image_count += 1
                    if image_count == 1:
                        print(f"We took {image_count} Picture ")
                    else:
                        print(f"We took {image_count} Pictures ")

                elif not face_locations:
                    i=6
                    while i>=0:
                        print(f'{i} seconds remaining...')
                        time.sleep(1)
                        i-=1
                        ret, frame = cap.read()
                        face_locations = face_recognition.face_locations(frame)
                        if face_locations:
                            break
                        elif i==0:
                            print('Time is up..please register again')
                            time.sleep(1)
                            shutil.rmtree(user_folder_path)
                            return


            cap.release()
            cv2.destroyAllWindows()

            known_face_encodings = load_images_and_encode_faces(user_folder_path, name)
            known_face_encodings_dict[name] = known_face_encodings

            print(f"User {name} has been registered successfully.")
        


def remove_user(dataset_path, known_face_encodings_dict, master_folder, normal_user_folder):
    master_user_encodings = load_master_user_encodings(master_folder)
    if master_user_encodings:
        known_face_encodings_dict["Master"] = master_user_encodings
    else:
            print("there is no master please register first.")
            return

    # Load master user encodings if master exists
    if master_user_encodings:
        known_master_encodings = load_master_user_encodings(master_folder)

        # Open the camera to recognize the user immediately
        cap = cv2.VideoCapture(0)
        cap.set(3, 680 * 2)
        cap.set(4, 480 * 2)

        recognized_user = False

        while not recognized_user:
            ret, frame = cap.read()

            # Recognize user using face encodings
            face_encodings = face_recognition.face_encodings(frame)
            for face_encoding in face_encodings:
                matches = face_recognition.compare_faces(known_master_encodings, face_encoding)
                if True in matches:
                    recognized_user = True
                    print("Master user recognized.")
                    break

            if recognized_user:
                break

            cv2.imshow("User Recognition", frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

            # Check if there are faces other than the master
            face_locations = face_recognition.face_locations(frame)
            if len(face_locations) > 0:
                print("Unauthorized user detected. Please enter the password.")
                break

        cap.release()
        cv2.destroyAllWindows()
        
    if recognized_user:
        print("Proceeding to remove a user.")
    else:
        # Ask for the password if user not recognized
        admin_pass = str(input("Enter the password: "))

        if admin_pass != "admin":
            print("Incorrect password. Access denied.")
            return
    
    # List all users in the normal user folder
    users = [name for name in os.listdir(normal_user_folder) if os.path.isdir(os.path.join(normal_user_folder, name))]
    if users == [] :
        Master = [name for name in os.listdir(master_folder) if os.path.isdir(os.path.join(master_folder, name))]
        print(f"The Master:{Master}")

        # Ask for user selection to remove
        user_index = input("Enter 1 to remove the Master (0 to go back to the main menu): ")
        if user_index == '0':
            return  # Go back to the main menu
        elif user_index == '1':
                user_to_remove = Master[0]
                master_folder_path = os.path.join(master_folder, user_to_remove)
                shutil.rmtree(master_folder_path)
                print(f"User {user_to_remove} has been removed successfully.")
        else:
            print(f"Wrong index")
            return

    else:
        print("List of Users:")
        for i, user in enumerate(users, 1):
            print(f"{i}. {user}")

        # Ask for user selection to remove
        user_index = input("Enter the index of the user to remove (0 to go back to the main menu): ")
        if user_index == '0':
            return  # Go back to the main menu
        else:
            user_index = int(user_index) - 1
            if 0 <= user_index < len(users):
                user_to_remove = users[user_index]
                user_folder_path = os.path.join(normal_user_folder, user_to_remove)
                shutil.rmtree(user_folder_path)
                print(f"User {user_to_remove} has been removed successfully.")
        
         


def switch_master_folder(master_folder, normal_user_folder):
    master_user_encodings = load_master_user_encodings(master_folder)
    if master_user_encodings:
        known_face_encodings_dict["Master"] = master_user_encodings
    else:

        print("there is no master please register first.")
        return

    # Load master user encodings if master exists
    if master_user_encodings:
        known_master_encodings = load_master_user_encodings(master_folder)

        # Open the camera to recognize the user immediately
        cap = cv2.VideoCapture(0)
        cap.set(3, 680 * 2)
        cap.set(4, 480 * 2)

        recognized_user = False

        while not recognized_user:
            ret, frame = cap.read()

            # Recognize user using face encodings
            face_encodings = face_recognition.face_encodings(frame)
            for face_encoding in face_encodings:
                matches = face_recognition.compare_faces(known_master_encodings, face_encoding)
                if True in matches:
                    recognized_user = True
                    print("Master user recognized.")
                    break

            if recognized_user:
                break

            cv2.imshow("User Recognition", frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

            # Check if there are faces other than the master
            face_locations = face_recognition.face_locations(frame)
            if len(face_locations) > 0:
                print("Unauthorized user detected.")
                break

        cap.release()
        cv2.destroyAllWindows()

    if recognized_user:
        if not os.path.exists(master_folder):
            print(f"Error: Master folder '{master_folder}' does not exist.")
            return
        if not os.path.exists(normal_user_folder):
            print(f"Error: Normal user folder '{normal_user_folder}' does not exist.")
            return

        master_users = os.listdir(master_folder)
        print("Master User:", master_users)

        normal_users = os.listdir(normal_user_folder)
        print("Normal Users:", normal_users)

        user_to_switch = input("Enter the [Master] Name: ")

        if user_to_switch not in master_users:
            print(f"Error: User '{user_to_switch}' not found in the master folder.")
            return

        user_folder_master = os.path.join(master_folder, user_to_switch)
        user_folder_normal = os.path.join(normal_user_folder, user_to_switch)
        shutil.move(user_folder_master, user_folder_normal)
        print(f"User folder '{user_to_switch}' switched from master to normal user folder successfully.")

        user_to_switch_normal = input("Enter the name of the user to switch folders with in the master folder: ")

        if user_to_switch_normal not in normal_users:
            print(f"Error: User '{user_to_switch_normal}' not found in the normal user folder.")
            user_folder_master = os.path.join(master_folder, user_to_switch)
            user_folder_normal = os.path.join(normal_user_folder, user_to_switch)
            shutil.move(user_folder_normal,user_folder_master)
            return

        user_folder_normal = os.path.join(normal_user_folder, user_to_switch_normal)
        user_folder_master = os.path.join(master_folder, user_to_switch_normal)
        shutil.move(user_folder_normal, user_folder_master)
        print(f"User folder '{user_to_switch_normal}' switched from normal user folder to master folder successfully.")
        
def Change_Password(master_folder):
    master_user_encodings = load_master_user_encodings(master_folder)
    if master_user_encodings:
        known_face_encodings_dict["Master"] = master_user_encodings
    else:
        print("There is no master please register first.")
        return

    # Load master user encodings if master exists
    if master_user_encodings:
        known_master_encodings = load_master_user_encodings(master_folder)

        # Open the camera to recognize the user immediately
        cap = cv2.VideoCapture(0)
        cap.set(3, 680 * 2)
        cap.set(4, 480 * 2)

        recognized_user = False

        while not recognized_user:
            ret, frame = cap.read()
            tries = 3
            # Recognize user using face encodings
            face_encodings = face_recognition.face_encodings(frame)
            for face_encoding in face_encodings:
                matches = face_recognition.compare_faces(known_master_encodings, face_encoding)
                if True in matches:
                    recognized_user = True
                    for passsword in range(tries):
                        new_password = input('Enter the new password (3-10 characters): ')
                        if len(new_password) in range(3,11) and new_password.isalnum() and new_password != default_password:
                            temp = new_password
                            new_password = input('Confirm the password: ')
                            if temp == new_password:
                                with open(password_file_path, 'w') as file:
                                    file.write(str(new_password))
                                print('Password changed successfully')
                                break
                            else: 
                                tries-=1
                                print(f'Confirmation failed..{tries} tries left')
                        else:
                            tries-=1
                            if new_password == default_password:
                                print(f'You must change the password..{tries} tries left')
                            elif tries!=0:
                                print(f'Try again..{tries} tries left')
                            elif tries == 0:
                                print('No tries left, the password remains the same.')
                                break

            if recognized_user:
                break

            cv2.imshow("User Recognition", frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

            # Check if there are faces other than the master
            face_locations = face_recognition.face_locations(frame)
            if len(face_locations) > 0:
                print("Unauthorized user detected.")
                break

        cap.release()
        cv2.destroyAllWindows()



def main():
    master_user = None
    known_face_encodings_dict = {}
    master_folder = os.path.join(dataset_path, "Master")
    normal_user_folder = os.path.join(dataset_path, "Normal Users")

    # Create the folders if they don't exist
    create_folders_if_not_exist(dataset_path, master_folder, normal_user_folder)

    while True:
        print("1. Join")
        print("2. Register")
        print("3. Remove User")
        print("4. Display List of Users")
        print("5. Switch Master")
        print("6. Change the password")
        print("7. End")
        master_user_encodings = load_master_user_encodings(master_folder)
        if master_user_encodings:
            known_face_encodings_dict["Master"] = master_user_encodings
        else:
            with open('default_password.txt', 'w') as file:
                file.write(str('1234'))
            print("{No users found, The default password is '1234'}")

        with open(password_file_path, 'r') as file:
            default_password = str(file.read())
        print(default_password) #for testing purposes comment it if you want
        

        choice = input("Enter your choice: ")

        if choice == "1":
            join_user(known_face_encodings_dict, master_folder, normal_user_folder)  
        elif choice == "2":
            register_user(dataset_path, known_face_encodings_dict, master_folder, normal_user_folder)
        elif choice == "3":
            remove_user(dataset_path, known_face_encodings_dict, master_folder, normal_user_folder)
        elif choice == "4":
            display_users(dataset_path,master_folder, normal_user_folder)
        elif choice == "5":
            switch_master_folder(master_folder, normal_user_folder)
        elif choice == "6":
            Change_Password(master_folder)
        elif choice == "7":
            print("Ending the program.")
            break
        else:
            print("Invalid choice. Please enter a valid option.")

if __name__ == "__main__":

    main()