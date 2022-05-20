#include "appender.h"
#include "crc/crc/crc32.h"

namespace ap{
    template<typename... args>
    static inline void A_LOG(const char *log, args... args1) {
        FILE *file = stderr;
        fprintf(file, log, args1...);
        fprintf(file, "\n");
        fflush(file);
    }

    template<typename... args>
    static inline void A_LOG_ERRNO(const char *log, args... args1) {
        FILE *file = stderr;
        fprintf(file, log, args1...);
        fprintf(file, ", err: %s\n", strerror(errno));
        fflush(file);
    }

    template<typename... args>
    static inline void A_LOG_TAG(const char *tag, const char *log, args... args1) {
        FILE *file = stderr;
        fprintf(file, "%s: ", tag);
        fprintf(file, log, args1...);
        fprintf(file, "\n");
        fflush(file);
    }

    template<typename... args>
    static inline void A_LOG_ERRNO_TAG(const char *tag, const char *log, args... args1) {
        FILE *file = stderr;
        fprintf(file, "%s: ", tag);
        fprintf(file, log, args1...);
        fprintf(file, ", err: %s\n", strerror(errno));
        fflush(file);
    }
    // view     : verify sign & CRC -> (convert ->) print
    // write    : (convert ->) cal. CRC -> copy -> write
    // delete   : verify sign & CRC -> copy

    static const char *SIGN = "\0APPEND\0";
    static const int SIGN_LEN = 8;

    static FILE *copyN_file_s(const char *des_path, const char *source_path, int size);
    static int copyN(FILE *des, FILE *source, int size);
    static int verify(FILE *file, char **to_content, int *to_size);

    char *ap_view(const char *filePath)
    {
        FILE *file;
        int result, size;
        char *content;
        file = fopen(filePath, "rb");

        if(!file)
        {
            A_LOG_ERRNO("Failed to open file");
            return nullptr;
        }
        result = verify(file, &content, &size);
        if (result)
        {
            A_LOG("Failed to verify");
            return nullptr;
        }
        A_LOG("Verification success, size: %d", size);
        return content;
    }

    int ap_write(const char *filePath, const char *newPath, const char *content)
    {
//        FILE *sourceFile, *genFile;
//        int size, result, crc;
//
//        sourceFile = fopen(filePath, "rb");
//        genFile = fopen(newPath, "ab");
//
//        fseek(sourceFile, 0, SEEK_END);
//        size = ftell(sourceFile);
//        fseek(sourceFile, 0, SEEK_SET);
//        result = copyN(genFile, sourceFile, size);
//
//        fclose(sourceFile);
        FILE *sourceFile, *genFile;
        int size, result, crc;

        sourceFile = fopen(filePath, "rb");
        if (!sourceFile)
        {
            A_LOG_ERRNO("Failed to open file");
            return -1;
        }

        fseek(sourceFile, 0, SEEK_END);
        size = ftell(sourceFile);
        fseek(sourceFile, 0, SEEK_CUR);
        if(errno)
        {
            A_LOG_ERRNO("Failed to copy to newFile");
            fclose(sourceFile);
            return -1;
        }
        fclose(sourceFile);

        genFile = copyN_file_s(newPath, filePath, size);

        if(!genFile)
        {
            A_LOG_ERRNO("Failed to copy to newFile");
            return -1;
        }

        // calculate meta info
        size = strlen(content)+1;
        crc = crc32(content, size);
        fwrite(&crc, 4, 1, genFile);
        fwrite(content, size, 1, genFile);
        fwrite(&size, 4, 1, genFile);
        fwrite(SIGN, 8, 1, genFile);

        if(errno)
        {
            A_LOG_ERRNO("Failed to write to file");
            fclose(genFile);
            return -1;
        }
        fclose(genFile);
        A_LOG("Success.");
        return 0;
    }

    int ap_delete(const char *filePath, const char *newPath)
    {
        FILE *sourceFile, *genFile;
        int size, result;

        sourceFile = fopen(filePath, "rb");
        if (!sourceFile){
            A_LOG_ERRNO("Failed to open file");
            return -1;
        }

        result = verify(sourceFile, nullptr, &size);
        if(result)
        {
            A_LOG("Failed to verify");
            fclose(sourceFile);
            return -1;
        }
        fseek(sourceFile, 0, SEEK_END);
        size = ftell(sourceFile) - size;
        fclose(sourceFile);
        genFile = copyN_file_s(newPath, filePath, size);
        if (!genFile)
        {
            A_LOG_ERRNO("Failed to write into file");
            return -1;
        }
        fclose(genFile);
        return 0;
    }

    // self cycle OK
    // without buffer
    // starts from current pos of $source
    static FILE *copyN_file_s(const char *des_path, const char *source_path, int size)
    {
        FILE *des, *source;
        char *tmp;
        source = fopen(source_path, "rb");
        if(!source)
            return nullptr;

        tmp = (char *)malloc(size);
        if(!tmp)
        {
            fclose(source);
            return nullptr;
        }
        fread(tmp, 1, size, source);
        if (errno)
        {
            fclose(source);
            return nullptr;
        }
        fclose(source);

        des = fopen(des_path, "wb");
        if (!des)
            return nullptr;
        fwrite(tmp, size, 1, des);
        if (errno)
        {
            fclose(des);
            return nullptr;
        }
        return des;
    }

    // without buffer
    // starts from current pos of $source
    static int copyN(FILE *des, FILE *source, int size)
    {
        char *tmp = (char *)malloc(size);
        fread(tmp, size, 1, source);
        if(errno)
        {
            free(tmp);
            return errno;
        }
        fwrite(tmp, size, 1, des);
        free(tmp);
        return errno;
    }

    static int verify(FILE *file, char **to_content, int *to_size)
    {
        int size;
        int crc;
        int ok;
        char *sign;
        char *content;
        // verify magic string "\0APPEND\0"
        sign = (char *)malloc(8);
        fseek(file, 0, SEEK_END);
        fseek(file, -8, SEEK_CUR);
        fread(sign, 8, 1, file);
        ok = errno || strcmp(sign+1, SIGN+1) != 0;
        free(sign);
        if(ok)
            return -1;

        // read size & content & crc
        fseek(file, 0, SEEK_END);
        fseek(file, -12, SEEK_CUR);
        fread(&size, 4, 1, file);

        if(errno)
            return errno;
        content = (char *)malloc(size);
        memset(content, 0, size);

        fseek(file, -size-8, SEEK_CUR);
        fread(&crc, 4, 1, file);
        fread(content, size, 1, file);
        if(errno)
        {
            free(content);
            return errno;
        }
        // verify CRC
        ok = crc != crc32(content, size);
        if(ok)
        {
            free(content);
            return -1;
        }
        // All passed.
        if(to_content)
            *to_content = content;
        else
            free(content);

        if(to_size)
            *to_size = size;

        return 0;
    }
}


