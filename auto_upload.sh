cd hector_localization 
git add .
git commit -m "$1"
git push origin +catkin
cd ../rotors_simulator
git add .
git commit -m "$1"
git push origin +master
cd ../rpg_svo
git add .
git commit -m "$1"
git push origin +master
cd ../rotors_utils
git add .
git commit -m "$1"
git push origin +master
cd ../Sophus
git add .
git commit -m "$1"
git push origin +master
cd ../sift
git add .
git commit -m "$1"
git push origin +master

cd ..
git add .
git commit -m "$1"
git push origin +master