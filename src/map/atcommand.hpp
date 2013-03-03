#ifndef ATCOMMAND_HPP
#define ATCOMMAND_HPP

#include "../common/const_array.hpp"

typedef struct AtCommandInfo
{
    const char *command;
    int level;
    int(*proc)(const int, struct map_session_data *,
            const char *command, const char *message);
} AtCommandInfo;

bool is_atcommand(const int fd, struct map_session_data *sd,
        const char *message, int gmlvl);

int get_atcommand_level(const AtCommandInfo *type);

int atcommand_config_read(const char *cfgName);

void log_atcommand(struct map_session_data *sd, const_string cmd);

// only used by map.cpp
extern char *gm_logfile_name;

#endif // ATCOMMAND_HPP
