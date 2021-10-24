/*
**
**   ▄████▄   ▄▄▄     ▄▄▄█████▓ ▄▄▄       ██▓   ▓██   ██▓  ██████ ▄▄▄█████▓
**  ▒██▀ ▀█  ▒████▄   ▓  ██▒ ▓▒▒████▄    ▓██▒    ▒██  ██▒▒██    ▒ ▓  ██▒ ▓▒
**  ▒▓█    ▄ ▒██  ▀█▄ ▒ ▓██░ ▒░▒██  ▀█▄  ▒██░     ▒██ ██░░ ▓██▄   ▒ ▓██░ ▒░
**  ▒▓▓▄ ▄██▒░██▄▄▄▄██░ ▓██▓ ░ ░██▄▄▄▄██ ▒██░     ░ ▐██▓░  ▒   ██▒░ ▓██▓ ░ 
**  ▒ ▓███▀ ░ ▓█   ▓██▒ ▒██▒ ░  ▓█   ▓██▒░██████▒ ░ ██▒▓░▒██████▒▒  ▒██▒ ░ 
**  ░ ░▒ ▒  ░ ▒▒   ▓▒█░ ▒ ░░    ▒▒   ▓▒█░░ ▒░▓  ░  ██▒▒▒ ▒ ▒▓▒ ▒ ░  ▒ ░░   
**    ░  ▒     ▒   ▒▒ ░   ░      ▒   ▒▒ ░░ ░ ▒  ░▓██ ░▒░ ░ ░▒  ░ ░    ░    
**    ░          ░   ▒    ░        ░   ▒     ░ ░   ▒ ▒ ░░  ░  ░  ░    ░      
**    ░ ░            ░  ░              ░  ░    ░  ░░ ░           ░           
**    ░                                            ░ ░                       
**
**  Copyright (c) 2021 - present, Santhosh K (0xCAFECODE)
**
**  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
**  files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
**  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
**  Software is furnished to do so, subject to the following conditions:
**
**  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
**  Software.
**
**  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
**  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
**  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
**  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
*/

#include <catalyst/compiler.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <catalyst/logging.h>

static catalyst_logger_t* default_logger = NULL;
static bool               default_constructed_logger = false;

static const char* log_level_color[] = {
    "\x1b[94m",
    "\x1b[36m",
    "\x1b[32m",
    "\x1b[33m",
    "\x1b[31m",
    "\x1b[35m"     
};

static const char* log_level_name[] = {
    "TRACE",
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
    "FATAL"     
};

typedef struct catalyst_logger_t {
    catalyst_logger_traits_t    traits;
    const char*                 format;
    va_list                     arguments;
    catalyst_log_level_t        log_level;
    int                         line;
    const char*                 file;
    struct tm*                  time;
} catalyst_logger_t;

static void logger_callback(catalyst_logger_t* logger) {
    char buffer[ 16 ];
    buffer[ strftime(buffer, 16, "%H:%M:%S", logger->time) ] = '\0';
    fprintf(logger->traits.sink, "%s | %s%s\x1b[0m \x1b[90m%s:%-5d\x1b[0m",
        buffer,
        log_level_color[logger->log_level], log_level_name[ logger->log_level ],
        logger->file, logger->line
    );
    vfprintf(logger->traits.sink, logger->format, logger->arguments);
    fprintf(logger->traits.sink, "\n");
    fflush(logger->traits.sink);
}

void  catalyst_initialize_logging() {
    default_logger  =   catalyst_create_logger((catalyst_logger_traits_t){
            .threshold_level = CATALYST_LOG_LEVEL_TRACE,
            .sink = stdout 
    });
    default_constructed_logger = true;
}

void catalyst_terminate_logging() {
    if(default_constructed_logger)
        catalyst_delete_logger(catalyst_logger_get_default());
}

catalyst_logger_t*  catalyst_create_logger(catalyst_logger_traits_t logger_traits) {
    catalyst_logger_t*  new_logger  =   malloc(sizeof (catalyst_logger_t));
    if(NULL == new_logger)
        return NULL;

    new_logger->traits              =   logger_traits;
    new_logger->time                = malloc(sizeof (struct tm));
    if(NULL == new_logger->time) {
        free(new_logger);
        return NULL;
    }

    return  new_logger;
}

void catalyst_delete_logger(catalyst_logger_t* logger) {
    fflush(logger->traits.sink);
    free(logger->time);
    free(logger);
}

void catalyst_logger_set_default(catalyst_logger_t* logger) {
    default_logger = logger;
}

catalyst_logger_t* catalyst_logger_get_default() {
    return default_logger;
}

catalyst_log_level_t    catalyst_logger_get_threshold_level(catalyst_logger_t* logger) {
    return logger->traits.threshold_level;
}

void                    catalyst_logger_set_threshold_level(catalyst_logger_t* logger, catalyst_log_level_t log_level) {
    logger->traits.threshold_level = log_level;
}

void                    catalyst_logger_set_sink(catalyst_logger_t* logger, void* sink) {
    logger->traits.sink = sink;
}

void*                   catalyst_logger_get_sink(catalyst_logger_t* logger) {
    return logger->traits.sink;
}
void catalyst_logger_log(
                            catalyst_log_level_t log_level, catalyst_logger_t* logger,
                            const char* file, int line,
                            const char* format, ...
                        ) {
    time_t  t = time(NULL);

    logger->format      =   format;
    logger->log_level   =   log_level;
    logger->line        =   line;
    logger->file        =   file;

#if defined(CATALYST_COMPILER_MSVC)
    localtime_s(logger->time, &t);
#else
    logger->time        =   localtime(&t);
#endif

    if(log_level >= logger->traits.threshold_level) {
        va_start(logger->arguments, format);
        logger_callback(logger);
        va_end(logger->arguments);
    }
}
