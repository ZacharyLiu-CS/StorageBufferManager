# Storage and Buffer Manager Project
## 1. before run this project , u need to check some libraries below:
```
cmake(minium version 3.10) g++(support c++11) gtest 
```
## 2. how to compile this project
```
mkdir -p build
cd build && cmake ..
make -j 
```
## 3. what the test meaning
```
Test_DMgr // this is used to test the functions of class DMgr
Test_BMgr // this is used to test the main function <FixPage> of class BMgr
Test_Total //this is used to test the total project which uses the zipf distribution requests to test the storag and buffer manager funciton
```
## 4. how to run the project
```
// make sure u are in the directory <build>
./Test_BMgr //if u want
./Test_DMgr // if u want
./Test_Total // if u want