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
#ifndef CATALYST_INCLUDE__LOGGER_H_
#define CATALYST_INCLUDE__LOGGER_H_

typedef enum {
    CATALYST_LOG_LEVEL_TRACE,   //
    CATALYST_LOG_LEVEL_DEBUG,   //
    CATALYST_LOG_LEVEL_INFO,    //
    CATALYST_LOG_LEVEL_WARN,    //
    CATALYST_LOG_LEVEL_ERROR,   //
    CATALYST_LOG_LEVEL_FATAL,   //
} catalyst_log_level_t;

typedef struct catalyst_logger_traits_t {
    catalyst_log_level_t    threshold_level;    //  The minimum level for logging
    void*                   sink;               //  Must be a FILE pointer
} catalyst_logger_traits_t;


typedef struct catalyst_logger_t catalyst_logger_t;

void                catalyst_initialize_logging();
void                catalyst_terminate_logging();

catalyst_logger_t*  catalyst_create_logger(catalyst_logger_traits_t logger_traits);
void                catalyst_delete_logger(catalyst_logger_t* logger);

void                catalyst_logger_set_default(catalyst_logger_t* logger);
catalyst_logger_t*  catalyst_logger_get_default();

catalyst_log_level_t catalyst_logger_get_threshold_level(catalyst_logger_t* logger);
void                catalyst_logger_set_threshold_level(catalyst_logger_t* logger, catalyst_log_level_t threshold_level);
void                catalyst_logger_set_sink(catalyst_logger_t* logger, void* sink);
void*               catalyst_logger_get_sink(catalyst_logger_t* logger);
void                catalyst_logger_log(
                                        catalyst_log_level_t log_level,
                                        catalyst_logger_t* logger,
                                        const char* file, int line,
                                        const char* format, ...
                                        );

#define CATALYST_LOG(logger, level, format, ...) catalyst_logger_log(level, logger, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define CATALYST_LOG_TRACE(logger, format, ...) CATALYST_LOG(logger, CATALYST_LOG_LEVEL_TRACE, format, ##__VA_ARGS__)
#define CATALYST_LOG_DEBUG(logger, format, ...) CATALYST_LOG(logger, CATALYST_LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define CATALYST_LOG_INFO(logger, format, ...) CATALYST_LOG(logger, CATALYST_LOG_LEVEL_INFO, format, ##__VA_ARGS__)
#define CATALYST_LOG_WARN(logger, format, ...) CATALYST_LOG(logger, CATALYST_LOG_LEVEL_WARN, format, ##__VA_ARGS__)
#define CATALYST_LOG_ERROR(logger, format, ...) CATALYST_LOG(logger, CATALYST_LOG_LEVEL_ERROR, format, ##__VA_ARGS__)
#define CATALYST_LOG_FATAL(logger, format, ...) CATALYST_LOG(logger, CATALYST_LOG_LEVEL_FATAL, format, ##__VA_ARGS__)

#define CATALYST_TRACE(...) CATALYST_LOG_TRACE(catalyst_logger_get_default(), ##__VA_ARGS__)
#define CATALYST_DEBUG(...) CATALYST_LOG_DEBUG(catalyst_logger_get_default(), ##__VA_ARGS__)
#define CATALYST_INFO(...) CATALYST_LOG_INFO(catalyst_logger_get_default(), ##__VA_ARGS__)
#define CATALYST_WARN(...) CATALYST_LOG_WARN(catalyst_logger_get_default(), ##__VA_ARGS__)
#define CATALYST_ERROR(...) CATALYST_LOG_ERROR(catalyst_logger_get_default(), ##__VA_ARGS__)
#define CATALYST_FATAL(...) CATALYST_LOG_FATAL(catalyst_logger_get_default(), ##__VA_ARGS__)

#define CATALYST_THRESHOLD_LEVEL(level) catalyst_logger_set_threshold_level(catalyst_logger_get_default(), level)

#endif
