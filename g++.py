import os , subprocess, json,glob

def run_binary(binary_filename,argv):
    print(f"Run binary : {binary_filename}\n")
    root = os.getcwd()+"/"+binary_filename
    subprocess.run([root,argv],check=True)

if __name__ == "__main__":
    run_binary("Access_Sensor","1")

    print("binary_run")