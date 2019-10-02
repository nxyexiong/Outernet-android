LOCAL_PATH := $(call my-dir)
ROOT_PATH := $(LOCAL_PATH)


########################################################
## libsodium
########################################################

include $(CLEAR_VARS)

SODIUM_SOURCE := \
	crypto_aead/aes256gcm/aesni/aead_aes256gcm_aesni.c \
	crypto_aead/chacha20poly1305/sodium/aead_chacha20poly1305.c \
	crypto_aead/xchacha20poly1305/sodium/aead_xchacha20poly1305.c \
	crypto_core/ed25519/ref10/ed25519_ref10.c \
	crypto_core/hchacha20/core_hchacha20.c \
	crypto_core/salsa/ref/core_salsa_ref.c \
	crypto_generichash/blake2b/ref/blake2b-compress-ref.c \
	crypto_generichash/blake2b/ref/blake2b-ref.c \
	crypto_generichash/blake2b/ref/generichash_blake2b.c \
	crypto_onetimeauth/poly1305/onetimeauth_poly1305.c \
	crypto_onetimeauth/poly1305/donna/poly1305_donna.c \
	crypto_pwhash/crypto_pwhash.c \
	crypto_pwhash/argon2/argon2-core.c \
	crypto_pwhash/argon2/argon2.c \
	crypto_pwhash/argon2/argon2-encoding.c \
	crypto_pwhash/argon2/argon2-fill-block-ref.c \
	crypto_pwhash/argon2/blake2b-long.c \
	crypto_pwhash/argon2/pwhash_argon2i.c \
	crypto_scalarmult/curve25519/scalarmult_curve25519.c \
	crypto_scalarmult/curve25519/ref10/x25519_ref10.c \
	crypto_stream/chacha20/stream_chacha20.c \
	crypto_stream/chacha20/ref/chacha20_ref.c \
	crypto_stream/salsa20/stream_salsa20.c \
	crypto_stream/salsa20/ref/salsa20_ref.c \
	crypto_verify/sodium/verify.c \
	randombytes/randombytes.c \
	randombytes/sysrandom/randombytes_sysrandom.c \
	sodium/core.c \
	sodium/runtime.c \
	sodium/utils.c \
	sodium/version.c

LOCAL_MODULE := sodium
LOCAL_CFLAGS += -I$(LOCAL_PATH)/libsodium/src/libsodium/include \
				-I$(LOCAL_PATH)/libsodium/src/libsodium/include/sodium \
				-DPACKAGE_NAME=\"libsodium\" -DPACKAGE_TARNAME=\"libsodium\" \
				-DPACKAGE_VERSION=\"1.0.15\" -DPACKAGE_STRING=\"libsodium-1.0.15\" \
				-DPACKAGE_BUGREPORT=\"https://github.com/jedisct1/libsodium/issues\" \
				-DPACKAGE_URL=\"https://github.com/jedisct1/libsodium\" \
				-DPACKAGE=\"libsodium\" -DVERSION=\"1.0.15\" \
				-DHAVE_PTHREAD=1                  \
				-DSTDC_HEADERS=1                  \
				-DHAVE_SYS_TYPES_H=1              \
				-DHAVE_SYS_STAT_H=1               \
				-DHAVE_STDLIB_H=1                 \
				-DHAVE_STRING_H=1                 \
				-DHAVE_MEMORY_H=1                 \
				-DHAVE_STRINGS_H=1                \
				-DHAVE_INTTYPES_H=1               \
				-DHAVE_STDINT_H=1                 \
				-DHAVE_UNISTD_H=1                 \
				-D__EXTENSIONS__=1                \
				-D_ALL_SOURCE=1                   \
				-D_GNU_SOURCE=1                   \
				-D_POSIX_PTHREAD_SEMANTICS=1      \
				-D_TANDEM_SOURCE=1                \
				-DHAVE_DLFCN_H=1                  \
				-DLT_OBJDIR=\".libs/\"            \
				-DHAVE_SYS_MMAN_H=1               \
				-DNATIVE_LITTLE_ENDIAN=1          \
				-DASM_HIDE_SYMBOL=.hidden         \
				-DHAVE_WEAK_SYMBOLS=1             \
				-DHAVE_ATOMIC_OPS=1               \
				-DHAVE_ARC4RANDOM=1               \
				-DHAVE_ARC4RANDOM_BUF=1           \
				-DHAVE_MMAP=1                     \
				-DHAVE_MLOCK=1                    \
				-DHAVE_MADVISE=1                  \
				-DHAVE_MPROTECT=1                 \
				-DHAVE_NANOSLEEP=1                \
				-DHAVE_POSIX_MEMALIGN=1           \
				-DHAVE_GETPID=1                   \
				-DCONFIGURED=1

LOCAL_SRC_FILES := $(addprefix libsodium/src/libsodium/,$(SODIUM_SOURCE))

include $(BUILD_STATIC_LIBRARY)

########################################################
## libev
########################################################

include $(CLEAR_VARS)

LOCAL_MODULE := libev
LOCAL_CFLAGS += -DNDEBUG -DHAVE_CONFIG_H \
				-I$(LOCAL_PATH)/libev
LOCAL_SRC_FILES := \
	libev/ev.c \
	libev/event.c

include $(BUILD_STATIC_LIBRARY)

########################################
## libclient
########################################

include $(CLEAR_VARS)

LOCAL_MODULE := libclient

LOCAL_C_INCLUDES := $(LOCAL_PATH)/libclient

CLIENT_SOURCES := client.cpp \
                  crypto.cpp \
                  buffer.cpp \
                  utils.cpp

LOCAL_SRC_FILES := $(addprefix libclient/, $(CLIENT_SOURCES))

LOCAL_CFLAGS := -Wall \
                -I$(LOCAL_PATH)/libev \
                -I$(LOCAL_PATH)/libsodium/src/libsodium/include

LOCAL_LDLIBS := -ldl -llog

LOCAL_STATIC_LIBRARIES := libev libsodium

include $(BUILD_SHARED_LIBRARY)

########################################
## libjni
########################################

include $(CLEAR_VARS)

LOCAL_MODULE := libjni

LOCAL_C_INCLUDES := $(LOCAL_PATH)/libjni

JNI_SOURCES := jni_caller.cpp

LOCAL_SRC_FILES := $(addprefix libjni/, $(JNI_SOURCES))

LOCAL_CFLAGS := -Werror=return-type \
                -I$(LOCAL_PATH)/libclient

LOCAL_LDLIBS := -ldl -llog

LOCAL_STATIC_LIBRARIES := libclient

include $(BUILD_SHARED_LIBRARY)