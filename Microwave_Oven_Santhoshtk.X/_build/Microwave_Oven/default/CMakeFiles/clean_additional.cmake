# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "/home/santhoshtk/MPLABXProjects/Microwave_Oven.X/out/Microwave_Oven/default.cmf"
  "/home/santhoshtk/MPLABXProjects/Microwave_Oven.X/out/Microwave_Oven/default.hex"
  "/home/santhoshtk/MPLABXProjects/Microwave_Oven.X/out/Microwave_Oven/default.hxl"
  "/home/santhoshtk/MPLABXProjects/Microwave_Oven.X/out/Microwave_Oven/default.mum"
  "/home/santhoshtk/MPLABXProjects/Microwave_Oven.X/out/Microwave_Oven/default.o"
  "/home/santhoshtk/MPLABXProjects/Microwave_Oven.X/out/Microwave_Oven/default.sdb"
  "/home/santhoshtk/MPLABXProjects/Microwave_Oven.X/out/Microwave_Oven/default.sym"
  )
endif()
