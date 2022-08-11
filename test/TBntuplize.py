import pydrcTB

filenames = [
    "cal_fast_1_117.dat",
    "cal_fast_2_117.dat",
    "cal_fast_3_117.dat",
    "cal_fast_4_117.dat",
    "cal_fast_5_117.dat",
    "cal_fast_6_117.dat",
    "cal_fast_7_117.dat",
    "cal_fast_8_117.dat",
    "cal_fast_9_117.dat",
    "cal_fast_10_117.dat",
    "cal_fast_11_117.dat",
    "cal_fast_12_117.dat",
    "cal_fast_13_117.dat",
    "cal_fast_14_117.dat",
    "cal_fast_15_117.dat"
]

reader = pydrcTB.TBread()
reader.ntuplizeFastmode(filenames,"test.root")
