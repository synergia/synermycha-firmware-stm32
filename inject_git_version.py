import subprocess

revision = (
    subprocess.check_output(["git", "describe", "--tags"])
    .decode("utf-8")
)
print("-DGIT_REV='\"%s\"'" % revision)
f = open("demofile2.txt", "a")
f.write("-DGIT_REV='\"%s\"'" % revision)
f.close()
