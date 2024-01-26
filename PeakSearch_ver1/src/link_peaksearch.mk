# object files linked for building exe files

# ROOT
OBJS_PEAKSEARCH += \
object/ControlFile.o \
object/OrthoPolynomial.o \
object/ProfileData.o \

# utility_rw_param
OBJS_PEAKSEARCH += \
object/utility_rw_param/I_ReadData.o \
object/utility_rw_param/RWParam_void.o \

# ver2
OBJS_PEAKSEARCH += \
object/ver2/akima.o \
object/ver2/ControlSmthData.o \
object/ver2/Deconvolution2.o \
object/ver2/main_peaksearch.o \
object/ver2/mu_sig.o \
object/ver2/PeakSearch.o \
object/ver2/SavitzkyGolay.o \


# utility_func
OBJS_PEAKSEARCH += \
object/utility_func/zmath.o \
object/utility_func/zstring.o \

# zerror_type
OBJS_PEAKSEARCH += \
object/zerror_type/error_mes.o \

# zlog
OBJS_PEAKSEARCH += \
object/zlog/rlog.o \
object/zlog/zlog.o \

