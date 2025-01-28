# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/lab4task1+Target_1"
  "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/1"
  "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/lab4task1+Target_1"
  "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/lab4task1+Target_1/tmp"
  "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/lab4task1+Target_1/src/lab4task1+Target_1-stamp"
  "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/lab4task1+Target_1/src"
  "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/lab4task1+Target_1/src/lab4task1+Target_1-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/lab4task1+Target_1/src/lab4task1+Target_1-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/dell/Desktop/2022_MC_58/Semester5/ES-II/2022_MC_58_Lab_4/LAB_Example/tmp/lab4task1+Target_1/src/lab4task1+Target_1-stamp${cfgdir}") # cfgdir has leading slash
endif()
