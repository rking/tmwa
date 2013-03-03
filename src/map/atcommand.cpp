#include "atcommand.hpp"

#include <cmath>
#include <cstring>
#include <ctime>

#include "../common/core.hpp"
#include "../common/cxxstdio.hpp"
#include "../common/mmo.hpp"
#include "../common/random.hpp"
#include "../common/nullpo.hpp"
#include "../common/socket.hpp"
#include "../common/timer.hpp"
#include "../common/utils2.hpp"

#include "battle.hpp"
#include "chrif.hpp"
#include "clif.hpp"
#include "intif.hpp"
#include "itemdb.hpp"
#include "map.hpp"
#include "mob.hpp"
#include "npc.hpp"
#include "party.hpp"
#include "pc.hpp"
#include "script.hpp"
#include "skill.hpp"
#include "storage.hpp"
#include "tmw.hpp"
#include "trade.hpp"

#include "../poison.hpp"

#define ATCOMMAND_FUNC(x) static \
int atcommand_##x(const int fd, struct map_session_data* sd, const char* command, const char* message)
ATCOMMAND_FUNC(setup);
ATCOMMAND_FUNC(broadcast);
ATCOMMAND_FUNC(localbroadcast);
ATCOMMAND_FUNC(charwarp);
ATCOMMAND_FUNC(warp);
ATCOMMAND_FUNC(where);
ATCOMMAND_FUNC(goto);
ATCOMMAND_FUNC(jump);
ATCOMMAND_FUNC(who);
ATCOMMAND_FUNC(whogroup);
ATCOMMAND_FUNC(whomap);
ATCOMMAND_FUNC(whomapgroup);
ATCOMMAND_FUNC(whogm);         // by Yor
ATCOMMAND_FUNC(save);
ATCOMMAND_FUNC(load);
ATCOMMAND_FUNC(speed);
ATCOMMAND_FUNC(storage);
ATCOMMAND_FUNC(option);
ATCOMMAND_FUNC(hide);
ATCOMMAND_FUNC(die);
ATCOMMAND_FUNC(kill);
ATCOMMAND_FUNC(alive);
ATCOMMAND_FUNC(kami);
ATCOMMAND_FUNC(heal);
ATCOMMAND_FUNC(item);
ATCOMMAND_FUNC(itemreset);
ATCOMMAND_FUNC(itemcheck);
ATCOMMAND_FUNC(baselevelup);
ATCOMMAND_FUNC(joblevelup);
ATCOMMAND_FUNC(help);
ATCOMMAND_FUNC(gm);
ATCOMMAND_FUNC(pvpoff);
ATCOMMAND_FUNC(pvpon);
ATCOMMAND_FUNC(model);
ATCOMMAND_FUNC(spawn);
ATCOMMAND_FUNC(killmonster);
ATCOMMAND_FUNC(killmonster2);
ATCOMMAND_FUNC(gat);
ATCOMMAND_FUNC(packet);
ATCOMMAND_FUNC(statuspoint);
ATCOMMAND_FUNC(skillpoint);
ATCOMMAND_FUNC(zeny);
template<ATTR attr>
ATCOMMAND_FUNC(param);
ATCOMMAND_FUNC(recall);
ATCOMMAND_FUNC(recallall);
ATCOMMAND_FUNC(revive);
ATCOMMAND_FUNC(character_stats);
ATCOMMAND_FUNC(character_stats_all);
ATCOMMAND_FUNC(character_option);
ATCOMMAND_FUNC(character_save);
ATCOMMAND_FUNC(doom);
ATCOMMAND_FUNC(doommap);
ATCOMMAND_FUNC(raise);
ATCOMMAND_FUNC(raisemap);
ATCOMMAND_FUNC(character_baselevel);
ATCOMMAND_FUNC(character_joblevel);
ATCOMMAND_FUNC(kick);
ATCOMMAND_FUNC(kickall);
ATCOMMAND_FUNC(questskill);
ATCOMMAND_FUNC(charquestskill);
ATCOMMAND_FUNC(lostskill);
ATCOMMAND_FUNC(charlostskill);
ATCOMMAND_FUNC(party);
ATCOMMAND_FUNC(charskreset);
ATCOMMAND_FUNC(charstreset);
ATCOMMAND_FUNC(charreset);
ATCOMMAND_FUNC(charstpoint);
ATCOMMAND_FUNC(charmodel);
ATCOMMAND_FUNC(charskpoint);
ATCOMMAND_FUNC(charzeny);
ATCOMMAND_FUNC(reloaditemdb);
ATCOMMAND_FUNC(reloadmobdb);
ATCOMMAND_FUNC(reloadskilldb);
ATCOMMAND_FUNC(reloadscript);
ATCOMMAND_FUNC(reloadgmdb);    // by Yor
ATCOMMAND_FUNC(mapexit);
ATCOMMAND_FUNC(idsearch);
ATCOMMAND_FUNC(mapinfo);
ATCOMMAND_FUNC(dye);           //** by fritz
ATCOMMAND_FUNC(hair_style);    //** by fritz
ATCOMMAND_FUNC(hair_color);    //** by fritz
ATCOMMAND_FUNC(all_stats);     //** by fritz
ATCOMMAND_FUNC(char_change_sex);   // by Yor
ATCOMMAND_FUNC(char_block);    // by Yor
ATCOMMAND_FUNC(char_ban);      // by Yor
ATCOMMAND_FUNC(char_unblock);  // by Yor
ATCOMMAND_FUNC(char_unban);    // by Yor
ATCOMMAND_FUNC(partyspy);      // [Syrus22]
ATCOMMAND_FUNC(partyrecall);   // by Yor
ATCOMMAND_FUNC(enablenpc);
ATCOMMAND_FUNC(disablenpc);
ATCOMMAND_FUNC(servertime);    // by Yor
ATCOMMAND_FUNC(chardelitem);   // by Yor
ATCOMMAND_FUNC(ignorelist);    // by Yor
ATCOMMAND_FUNC(charignorelist);    // by Yor
ATCOMMAND_FUNC(inall);         // by Yor
ATCOMMAND_FUNC(exall);         // by Yor
ATCOMMAND_FUNC(email);         // by Yor
ATCOMMAND_FUNC(effect);        //by Apple
ATCOMMAND_FUNC(character_item_list);   // by Yor
ATCOMMAND_FUNC(character_storage_list);    // by Yor
ATCOMMAND_FUNC(character_cart_list);   // by Yor
ATCOMMAND_FUNC(addwarp);       // by MouseJstr
ATCOMMAND_FUNC(killer);        // by MouseJstr
ATCOMMAND_FUNC(npcmove);       // by MouseJstr
ATCOMMAND_FUNC(killable);      // by MouseJstr
ATCOMMAND_FUNC(charkillable);  // by MouseJstr
ATCOMMAND_FUNC(chareffect);    // by MouseJstr
ATCOMMAND_FUNC(dropall);       // by MouseJstr
ATCOMMAND_FUNC(chardropall);   // by MouseJstr
ATCOMMAND_FUNC(storeall);      // by MouseJstr
ATCOMMAND_FUNC(charstoreall);  // by MouseJstr
ATCOMMAND_FUNC(summon);
ATCOMMAND_FUNC(rain);
ATCOMMAND_FUNC(snow);
ATCOMMAND_FUNC(sakura);
ATCOMMAND_FUNC(fog);
ATCOMMAND_FUNC(leaves);
ATCOMMAND_FUNC(adjgmlvl);      // by MouseJstr
ATCOMMAND_FUNC(adjcmdlvl);     // by MouseJstr
ATCOMMAND_FUNC(trade);         // by MouseJstr
ATCOMMAND_FUNC(char_wipe);     // [Fate]
ATCOMMAND_FUNC(set_magic);     // [Fate]
ATCOMMAND_FUNC(magic_info);    // [Fate]
ATCOMMAND_FUNC(log);           // [Fate]
ATCOMMAND_FUNC(tee);           // [Fate]
ATCOMMAND_FUNC(invisible);     // [Fate]
ATCOMMAND_FUNC(visible);       // [Fate]
ATCOMMAND_FUNC(list_nearby);   // [Fate]
ATCOMMAND_FUNC(iterate_forward_over_players);  // [Fate]
ATCOMMAND_FUNC(iterate_backwards_over_players);    // [Fate]
ATCOMMAND_FUNC(skillpool_info);    // [Fate]
ATCOMMAND_FUNC(skillpool_focus);   // [Fate]
ATCOMMAND_FUNC(skillpool_unfocus); // [Fate]
ATCOMMAND_FUNC(skill_learn);   // [Fate]
ATCOMMAND_FUNC(wgm);
ATCOMMAND_FUNC(ipcheck);
ATCOMMAND_FUNC(doomspot);

/*==========================================
 *AtCommandInfo atcommand_info[]構造体の定義
 *------------------------------------------
 */

// First char of commands is configured in atcommand_athena.conf. Leave @ in this list for default value.
// to set default level, read atcommand_athena.conf first please.
static
AtCommandInfo atcommand_info[] = {
    {"@setup", 40, atcommand_setup},
    {"@charwarp", 60, atcommand_charwarp},
    {"@warp", 40, atcommand_warp},
    {"@where", 1, atcommand_where},
    {"@goto", 20, atcommand_goto},
    {"@jump", 40, atcommand_jump},
    {"@who", 20, atcommand_who},
    {"@whogroup", 20, atcommand_whogroup},
    {"@whomap", 20, atcommand_whomap},
    {"@whomapgroup", 20, atcommand_whomapgroup},
    {"@whogm", 20, atcommand_whogm},   // by Yor
    {"@save", 40, atcommand_save},
    {"@return", 40, atcommand_load},
    {"@load", 40, atcommand_load},
    {"@speed", 40, atcommand_speed},
    {"@storage", 1, atcommand_storage},
    {"@option", 40, atcommand_option},
    {"@hide", 40, atcommand_hide},  // + /hide
    {"@die", 1, atcommand_die},
    {"@kill", 60, atcommand_kill},
    {"@alive", 60, atcommand_alive},
    {"@kami", 40, atcommand_kami},
    {"@heal", 40, atcommand_heal},
    {"@item", 60, atcommand_item},
    {"@itemreset", 40, atcommand_itemreset},
    {"@itemcheck", 60, atcommand_itemcheck},
    {"@blvl", 60, atcommand_baselevelup},
    {"@jlvl", 60, atcommand_joblevelup},
    {"@help", 20, atcommand_help},
    {"@gm", 100, atcommand_gm},
    {"@pvpoff", 40, atcommand_pvpoff},
    {"@pvpon", 40, atcommand_pvpon},
    {"@model", 20, atcommand_model},
    {"@spawn", 50, atcommand_spawn},
    {"@killmonster", 60, atcommand_killmonster},
    {"@killmonster2", 40, atcommand_killmonster2},
    {"@gat", 99, atcommand_gat}, // debug function
    {"@packet", 99, atcommand_packet},    // debug function
    {"@stpoint", 60, atcommand_statuspoint},
    {"@skpoint", 60, atcommand_skillpoint},
    {"@zeny", 60, atcommand_zeny},
    {"@str", 60, atcommand_param<ATTR::STR>},
    {"@agi", 60, atcommand_param<ATTR::AGI>},
    {"@vit", 60, atcommand_param<ATTR::VIT>},
    {"@int", 60, atcommand_param<ATTR::INT>},
    {"@dex", 60, atcommand_param<ATTR::DEX>},
    {"@luk", 60, atcommand_param<ATTR::LUK>},
    {"@recall", 60, atcommand_recall},    // + /recall
    {"@revive", 60, atcommand_revive},
    {"@charstats", 40, atcommand_character_stats},
    {"@charstatsall", 40, atcommand_character_stats_all},
    {"@charoption", 60, atcommand_character_option},
    {"@charsave", 60, atcommand_character_save},
    {"@doom", 80, atcommand_doom},
    {"@doommap", 80, atcommand_doommap},
    {"@raise", 80, atcommand_raise},
    {"@raisemap", 80, atcommand_raisemap},
    {"@charbaselvl", 60, atcommand_character_baselevel},
    {"@charjlvl", 60, atcommand_character_joblevel},
    {"@kick", 20, atcommand_kick},  // + right click menu for GM "(name) force to quit"
    {"@kickall", 99, atcommand_kickall},
    {"@questskill", 40, atcommand_questskill},
    {"@charquestskill", 60, atcommand_charquestskill},
    {"@lostskill", 40, atcommand_lostskill},
    {"@charlostskill", 60, atcommand_charlostskill},
    {"@party", 1, atcommand_party},
    {"@mapexit", 99, atcommand_mapexit},
    {"@idsearch", 60, atcommand_idsearch},
    {"@mapmove", 40, atcommand_warp},    // /mm command
    {"@broadcast", 40, atcommand_broadcast},   // /b and /nb command
    {"@localbroadcast", 40, atcommand_localbroadcast},    // /lb and /nlb command
    {"@recallall", 80, atcommand_recallall},
    {"@charskreset", 60, atcommand_charskreset},
    {"@charstreset", 60, atcommand_charstreset},
    {"@reloaditemdb", 99, atcommand_reloaditemdb},  // admin command
    {"@reloadmobdb", 99, atcommand_reloadmobdb}, // admin command
    {"@reloadskilldb", 99, atcommand_reloadskilldb},   // admin command
    {"@reloadscript", 99, atcommand_reloadscript},  // admin command
    {"@reloadgmdb", 99, atcommand_reloadgmdb},    // admin command
    {"@charreset", 60, atcommand_charreset},
    {"@charmodel", 50, atcommand_charmodel},
    {"@charskpoint", 60, atcommand_charskpoint},
    {"@charstpoint", 60, atcommand_charstpoint},
    {"@charzeny", 60, atcommand_charzeny},
    {"@mapinfo", 99, atcommand_mapinfo},
    {"@dye", 40, atcommand_dye}, // by fritz
    {"@ccolor", 40, atcommand_dye},  // by fritz
    {"@hairstyle", 40, atcommand_hair_style},  // by fritz
    {"@haircolor", 40, atcommand_hair_color},  // by fritz
    {"@allstats", 60, atcommand_all_stats}, // by fritz
    {"@charchangesex", 60, atcommand_char_change_sex}, // by Yor
    {"@block", 60, atcommand_char_block},  // by Yor
    {"@unblock", 60, atcommand_char_unblock},    // by Yor
    {"@ban", 60, atcommand_char_ban},    // by Yor
    {"@unban", 60, atcommand_char_unban},  // by Yor
    {"@partyspy", 60, atcommand_partyspy},  // [Syrus22]
    {"@partyrecall", 60, atcommand_partyrecall}, // by Yor
    {"@enablenpc", 80, atcommand_enablenpc},   // []
    {"@disablenpc", 80, atcommand_disablenpc},    // []
    {"@servertime", 0, atcommand_servertime}, // by Yor
    {"@chardelitem", 60, atcommand_chardelitem}, // by Yor
    {"@listnearby", 40, atcommand_list_nearby},   // by Yor
    {"@ignorelist", 0, atcommand_ignorelist}, // by Yor
    {"@charignorelist", 20, atcommand_charignorelist},    // by Yor
    {"@inall", 20, atcommand_inall},  // by Yor
    {"@exall", 20, atcommand_exall},   // by Yor
    {"@email", 0, atcommand_email},    // by Yor
    {"@effect", 40, atcommand_effect},    // by Apple
    {"@charitemlist", 40, atcommand_character_item_list}, // by Yor
    {"@charstoragelist", 40, atcommand_character_storage_list},    // by Yor
    {"@charcartlist", 40, atcommand_character_cart_list}, // by Yor
    {"@addwarp", 20, atcommand_addwarp}, // by MouseJstr
    {"@killer", 60, atcommand_killer},    // by MouseJstr
    {"@npcmove", 20, atcommand_npcmove}, // by MouseJstr
    {"@killable", 40, atcommand_killable},  // by MouseJstr
    {"@charkillable", 40, atcommand_charkillable},  // by MouseJstr
    {"@chareffect", 40, atcommand_chareffect},    // MouseJstr
    {"@dropall", 40, atcommand_dropall}, // MouseJstr
    {"@chardropall", 40, atcommand_chardropall}, // MouseJstr
    {"@storeall", 40, atcommand_storeall},  // MouseJstr
    {"@charstoreall", 40, atcommand_charstoreall},  // MouseJstr
    {"@rain", 99, atcommand_rain},
    {"@snow", 99, atcommand_snow},
    {"@sakura", 99, atcommand_sakura},
    {"@fog", 99, atcommand_fog},
    {"@leaves", 99, atcommand_leaves},
    {"@summon", 60, atcommand_summon},
    {"@adjgmlvl", 99, atcommand_adjgmlvl},
    {"@adjcmdlvl", 99, atcommand_adjcmdlvl},
    {"@trade", 60, atcommand_trade},
    {"@charwipe", 60, atcommand_char_wipe},   // [Fate]
    {"@setmagic", 99, atcommand_set_magic}, // [Fate]
    {"@magicinfo", 60, atcommand_magic_info},  // [Fate]
    {"@log", 60, atcommand_log}, // [Fate]
    {"@l", 60, atcommand_log},   // [Fate]
    {"@tee", 60, atcommand_tee}, // [Fate]
    {"@t", 60, atcommand_tee},   // [Fate]
    {"@invisible", 60, atcommand_invisible},   // [Fate]
    {"@visible", 60, atcommand_visible}, // [Fate]
    {"@hugo", 60, atcommand_iterate_forward_over_players},    // [Fate]
    {"@linus", 60, atcommand_iterate_backwards_over_players},    // [Fate]
    {"@sp-info", 40, atcommand_skillpool_info},  // [Fate]
    {"@sp-focus", 80, atcommand_skillpool_focus},    // [Fate]
    {"@sp-unfocus", 80, atcommand_skillpool_unfocus},    // [Fate]
    {"@skill-learn", 80, atcommand_skill_learn}, // [Fate]
    {"@wgm", 0, atcommand_wgm},
    {"@ipcheck", 60, atcommand_ipcheck},
    {"@doomspot", 60, atcommand_doomspot},

// add new commands before this line
    {NULL, 1, NULL}
};

/*==========================================
 * get_atcommand_level @コマンドの必要レベルを取得
 *------------------------------------------
 */
int get_atcommand_level(const AtCommandInfo *type)
{
    if (type)
        return type->level;

    return 100;                 // 100: command can not be used
}

static
FILE *get_gm_log();

/*========================================
 * At-command logging
 */
void log_atcommand(struct map_session_data *sd, const_string cmd)
{
    FILE *fp = get_gm_log();
    if (!fp)
        return;
    timestamp_seconds_buffer tmpstr;
    stamp_time(tmpstr);
    fprintf(fp, "[%s] %s(%d,%d) %s(%d) : ",
            tmpstr,
            map[sd->bl.m].name, sd->bl.x, sd->bl.y,
            sd->status.name, sd->status.account_id);
    fwrite(cmd.data(), 1, cmd.size(), fp);
}

char *gm_logfile_name = NULL;
/*==========================================
 * Log a timestamped line to GM log file
 *------------------------------------------
 */
FILE *get_gm_log()
{
    if (!gm_logfile_name)
        return NULL;

    time_t ts = time(NULL);
    struct tm ctime = *gmtime(&ts);

    int year = ctime.tm_year + 1900;
    int month = ctime.tm_mon + 1;
    int logfile_nr = (year * 12) + month;

    static FILE *gm_logfile = NULL;
    static int last_logfile_nr = 0;
    if (logfile_nr == last_logfile_nr)
        return gm_logfile;
    last_logfile_nr = logfile_nr;

    std::string fullname = STRPRINTF("%s.%04d-%02d",
            gm_logfile_name, year, month);

    if (gm_logfile)
        fclose_(gm_logfile);

    gm_logfile = fopen_(fullname.c_str(), "a");

    if (!gm_logfile)
    {
        perror("GM log file");
        gm_logfile_name = NULL;
    }
    return gm_logfile;
}

static
AtCommandInfo *atcommand(const int level, const char *message);
/*==========================================
 *is_atcommand @コマンドに存在するかどうか確認する
 *------------------------------------------
 */
bool is_atcommand(const int fd, struct map_session_data *sd,
        const char *message, int gmlvl)
{
    nullpo_retr(false, sd);

    if (!message || !*message)
        return false;

    AtCommandInfo *info = atcommand(gmlvl > 0 ? gmlvl : pc_isGM(sd), message);
    if (info)
    {
        char command[100];
        const char *str = message;
        const char *p = message;
        memset(command, '\0', sizeof(command));
        while (*p && !isspace(*p))
            p++;
        if (p - str >= sizeof(command))    // too long
            return true;
        strncpy(command, str, p - str);
        while (isspace(*p))
            p++;

        {
            if (info->proc(fd, sd, command, p) != 0)
            {
                // Command can not be executed
                std::string output = STRPRINTF("%s failed.", command);
                clif_displaymessage(fd, output);
            }
            else
            {
                if (get_atcommand_level(info) != 0)    // Don't log level 0 commands
                    log_atcommand(sd, message);
            }
        }

        return true;
    }

    return false;
}

/*==========================================
 *
 *------------------------------------------
 */
AtCommandInfo *atcommand(const int level, const char *message)
{
    const char *p = message;

    if (battle_config.atc_gmonly != 0 && !level)    // level = pc_isGM(sd)
        return nullptr;
    if (!p || !*p)
    {
        FPRINTF(stderr, "at command message is empty\n");
        return nullptr;
    }

    if (*p == '@')
    {                           // check first char.
        char command[101];
        int i = 0;
        sscanf(p, "%100s", command);
        command[sizeof(command) - 1] = '\0';

        while (atcommand_info[i].command)
        {
            if (strcasecmp(command, atcommand_info[i].command) == 0
                && level >= atcommand_info[i].level)
            {
                return &atcommand_info[i];
            }
            i++;
        }

        {
            // doesn't return Unknown if player is normal player (display the text, not display: unknown command)
            if (level == 0)
                return nullptr;
            else
                return nullptr;
        }
    }
    else
    {
        return nullptr;
    }

}

/*==========================================
 *
 *------------------------------------------
 */
static
void atkillmonster_sub(struct block_list *bl, int flag)
{
    nullpo_retv(bl);

    if (flag)
        mob_damage(NULL, (struct mob_data *) bl,
                    ((struct mob_data *) bl)->hp, 2);
    else
        mob_delete((struct mob_data *) bl);
}

/*==========================================
 *
 *------------------------------------------
 */
static
AtCommandInfo *get_atcommandinfo_byname(const char *name)
{
    int i;

    for (i = 0; atcommand_info[i].command; i++)
        if (strcasecmp(atcommand_info[i].command + 1, name) == 0)
            return &atcommand_info[i];

    return NULL;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_config_read(const char *cfgName)
{
    char line[1024], w1[1024], w2[1024];
    AtCommandInfo *p;
    FILE *fp;

    if ((fp = fopen_(cfgName, "r")) == NULL)
    {
        PRINTF("At commands configuration file not found: %s\n", cfgName);
        return 1;
    }

    while (fgets(line, sizeof(line) - 1, fp))
    {
        if (line[0] == '/' && line[1] == '/')
            continue;

        if (sscanf(line, "%1023[^:]:%1023s", w1, w2) != 2)
            continue;
        p = get_atcommandinfo_byname(w1);
        if (p != NULL)
        {
            p->level = atoi(w2);
            if (p->level > 100)
                p->level = 100;
            else if (p->level < 0)
                p->level = 0;
        }

        if (strcasecmp(w1, "import") == 0)
            atcommand_config_read(w2);
    }
    fclose_(fp);

    return 0;
}

/*==========================================
// @ command processing functions
 *------------------------------------------
 */

/*==========================================
 * @setup - Safely set a chars levels and warp them to a special place
 * TAW Specific
 *------------------------------------------
 */
int atcommand_setup(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    char character[100];
    int level = 1;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &level, character) < 2)
    {
        clif_displaymessage(fd, "Usage: @setup <level> <char name>");
        return -1;
    }
    level--;

    std::string buf;
    buf = STRPRINTF("-255 %s", character);
    atcommand_character_baselevel(fd, sd, "@charbaselvl", buf.c_str());

    buf = STRPRINTF("%d %s", level, character);
    atcommand_character_baselevel(fd, sd, "@charbaselvl", buf.c_str());

    // Emote skill
    buf = STRPRINTF("1 1 %s", character);
    atcommand_skill_learn(fd, sd, "@skill-learn", buf.c_str());

    // Trade skill
    buf = STRPRINTF("2 1 %s", character);
    atcommand_skill_learn(fd, sd, "@skill-learn", buf.c_str());

    // Party skill
    STRPRINTF("2 2 %s", character);
    atcommand_skill_learn(fd, sd, "@skill-learn", buf.c_str());

    STRPRINTF("018-1.gat 24 98 %s", character);
    atcommand_charwarp(fd, sd, "@charwarp", buf.c_str());

    return (0);

}

/*==========================================
 * @rura+
 *------------------------------------------
 */
int atcommand_charwarp(const int fd, struct map_session_data *sd,
                        const char *, const char *message)
{
    char map_name[100];
    char character[100];
    int x = 0, y = 0;
    struct map_session_data *pl_sd;
    int m;

    memset(map_name, '\0', sizeof(map_name));
    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%99s %d %d %99[^\n]", map_name, &x, &y,
                   character) < 4)
    {
        clif_displaymessage(fd,
                             "Usage: @charwarp/@rura+ <mapname> <x> <y> <char name>");
        return -1;
    }

    if (x <= 0)
        x = random_::in(1, 399);
    if (y <= 0)
        y = random_::in(1, 399);
    if (strstr(map_name, ".gat") == NULL && strstr(map_name, ".afm") == NULL && strlen(map_name) < 13)   // 16 - 4 (.gat)
        strcat(map_name, ".gat");

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can rura+ only lower or same GM level
            if (x > 0 && x < 800 && y > 0 && y < 800)
            {
                m = map_mapname2mapid(map_name);
                if (m >= 0 && map[m].flag.nowarpto
                    && battle_config.any_warp_GM_min_level > pc_isGM(sd))
                {
                    clif_displaymessage(fd,
                                         "You are not authorised to warp someone to this map.");
                    return -1;
                }
                if (pl_sd->bl.m >= 0 && map[pl_sd->bl.m].flag.nowarp
                    && battle_config.any_warp_GM_min_level > pc_isGM(sd))
                {
                    clif_displaymessage(fd,
                                         "You are not authorised to warp this player from its actual map.");
                    return -1;
                }
                if (pc_setpos(pl_sd, map_name, x, y, BeingRemoveWhy::WARPED) == 0)
                {
                    clif_displaymessage(pl_sd->fd, "Warped.");
                    clif_displaymessage(fd, "Player warped (message sends to player too).");
                }
                else
                {
                    clif_displaymessage(fd, "Map not found.");
                    return -1;
                }
            }
            else
            {
                clif_displaymessage(fd, "Coordinates out of range.");
                return -1;
            }
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_warp(const int fd, struct map_session_data *sd,
                    const char *, const char *message)
{
    char map_name[100];
    int x = 0, y = 0;
    int m;

    memset(map_name, '\0', sizeof(map_name));

    if (!message || !*message
        || sscanf(message, "%99s %d %d", map_name, &x, &y) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a map (usage: @warp <mapname> <x> <y>).");
        return -1;
    }

    if (x <= 0)
        x = random_::in(1, 399);
    if (y <= 0)
        y = random_::in(1, 399);

    if (strstr(map_name, ".gat") == NULL && strstr(map_name, ".afm") == NULL && strlen(map_name) < 13)   // 16 - 4 (.gat)
        strcat(map_name, ".gat");

    if (x > 0 && x < 800 && y > 0 && y < 800)
    {
        m = map_mapname2mapid(map_name);
        if (m >= 0 && map[m].flag.nowarpto
            && battle_config.any_warp_GM_min_level > pc_isGM(sd))
        {
            clif_displaymessage(fd,
                                 "You are not authorised to warp you to this map.");
            return -1;
        }
        if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarp
            && battle_config.any_warp_GM_min_level > pc_isGM(sd))
        {
            clif_displaymessage(fd,
                                 "You are not authorised to warp you from your actual map.");
            return -1;
        }
        if (pc_setpos(sd, map_name, x, y, BeingRemoveWhy::WARPED) == 0)
            clif_displaymessage(fd, "Warped.");
        else
        {
            clif_displaymessage(fd, "Map not found.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Coordinates out of range.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_where(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (sscanf(message, "%99[^\n]", character) < 1)
        strcpy(character, sd->status.name);

    if ((pl_sd = map_nick2sd(character)) != NULL &&
        !((battle_config.hide_GM_session
           || bool(pl_sd->status.option & Option::HIDE))
          && (pc_isGM(pl_sd) > pc_isGM(sd))))
    {                           // you can look only lower or same level
        std::string output = STRPRINTF("%s: %s (%d,%d)",
                pl_sd->status.name,
                pl_sd->mapname, pl_sd->bl.x, pl_sd->bl.y);
        clif_displaymessage(fd, output);
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_goto(const int fd, struct map_session_data *sd,
                    const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @jumpto/@warpto/@goto <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pl_sd->bl.m >= 0 && map[pl_sd->bl.m].flag.nowarpto
            && battle_config.any_warp_GM_min_level > pc_isGM(sd))
        {
            clif_displaymessage(fd,
                                 "You are not authorised to warp you to the map of this player.");
            return -1;
        }
        if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarp
            && battle_config.any_warp_GM_min_level > pc_isGM(sd))
        {
            clif_displaymessage(fd,
                                 "You are not authorised to warp you from your actual map.");
            return -1;
        }
        pc_setpos(sd, pl_sd->mapname, pl_sd->bl.x, pl_sd->bl.y, BeingRemoveWhy::WARPED);
        std::string output = STRPRINTF("Jump to %s", character);
        clif_displaymessage(fd, output);
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_jump(const int fd, struct map_session_data *sd,
                    const char *, const char *message)
{
    int x = 0, y = 0;
    sscanf(message, "%d %d", &x, &y);

    if (x <= 0)
        x = random_::in(1, 399);
    if (y <= 0)
        y = random_::in(1, 399);
    if (x > 0 && x < 800 && y > 0 && y < 800)
    {
        if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarpto
            && battle_config.any_warp_GM_min_level > pc_isGM(sd))
        {
            clif_displaymessage(fd,
                                 "You are not authorised to warp you to your actual map.");
            return -1;
        }
        if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarp
            && battle_config.any_warp_GM_min_level > pc_isGM(sd))
        {
            clif_displaymessage(fd,
                                 "You are not authorised to warp you from your actual map.");
            return -1;
        }
        pc_setpos(sd, sd->mapname, x, y, BeingRemoveWhy::WARPED);
        std::string output = STRPRINTF("Jump to %d %d", x, y);
        clif_displaymessage(fd, output);
    }
    else
    {
        clif_displaymessage(fd, "Coordinates out of range.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_who(const int fd, struct map_session_data *sd,
                   const char *, const char *message)
{
    struct map_session_data *pl_sd;
    int i, j, count;
    int pl_GM_level, GM_level;
    char match_text[100];
    char player_name[24];

    memset(match_text, '\0', sizeof(match_text));
    memset(player_name, '\0', sizeof(player_name));

    if (sscanf(message, "%99[^\n]", match_text) < 1)
        strcpy(match_text, "");
    for (j = 0; match_text[j]; j++)
        match_text[j] = tolower(match_text[j]);

    count = 0;
    GM_level = pc_isGM(sd);
    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth)
        {
            pl_GM_level = pc_isGM(pl_sd);
            if (!
                ((battle_config.hide_GM_session
                  || bool(pl_sd->status.option & Option::HIDE))
                 && (pl_GM_level > GM_level)))
            {                   // you can look only lower or same level
                memcpy(player_name, pl_sd->status.name, 24);
                for (j = 0; player_name[j]; j++)
                    player_name[j] = tolower(player_name[j]);
                if (strstr(player_name, match_text) != NULL)
                {
                    // search with no case sensitive
                    std::string output;
                    if (pl_GM_level > 0)
                        output = STRPRINTF(
                                "Name: %s (GM:%d) | Location: %s %d %d",
                                pl_sd->status.name, pl_GM_level,
                                pl_sd->mapname, pl_sd->bl.x, pl_sd->bl.y);
                    else
                        output = STRPRINTF(
                                "Name: %s | Location: %s %d %d",
                                pl_sd->status.name, pl_sd->mapname,
                                pl_sd->bl.x, pl_sd->bl.y);
                    clif_displaymessage(fd, output);
                    count++;
                }
            }
        }
    }

    if (count == 0)
        clif_displaymessage(fd, "No player found.");
    else if (count == 1)
        clif_displaymessage(fd, "1 player found.");
    else
    {
        std::string output = STRPRINTF("%d players found.", count);
        clif_displaymessage(fd, output);
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_whogroup(const int fd, struct map_session_data *sd,
                        const char *, const char *message)
{
    struct map_session_data *pl_sd;
    int i, j, count;
    int pl_GM_level, GM_level;
    char match_text[100];
    char player_name[24];
    struct party *p;

    memset(match_text, '\0', sizeof(match_text));
    memset(player_name, '\0', sizeof(player_name));

    if (sscanf(message, "%99[^\n]", match_text) < 1)
        strcpy(match_text, "");
    for (j = 0; match_text[j]; j++)
        match_text[j] = tolower(match_text[j]);

    count = 0;
    GM_level = pc_isGM(sd);
    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth)
        {
            pl_GM_level = pc_isGM(pl_sd);
            if (!
                ((battle_config.hide_GM_session
                  || bool(pl_sd->status.option & Option::HIDE))
                 && (pl_GM_level > GM_level)))
            {                   // you can look only lower or same level
                memcpy(player_name, pl_sd->status.name, 24);
                for (j = 0; player_name[j]; j++)
                    player_name[j] = tolower(player_name[j]);
                if (strstr(player_name, match_text) != NULL)
                {               // search with no case sensitive
                    p = party_search(pl_sd->status.party_id);
                    const char *temp0 = p ? p->name : "None";
                    std::string output;
                    if (pl_GM_level > 0)
                        output = STRPRINTF(
                                "Name: %s (GM:%d) | Party: '%s'",
                                pl_sd->status.name, pl_GM_level, temp0);
                    clif_displaymessage(fd, output);
                    count++;
                }
            }
        }
    }

    if (count == 0)
        clif_displaymessage(fd, "No player found.");
    else if (count == 1)
        clif_displaymessage(fd, "1 player found.");
    else
    {
        std::string output = STRPRINTF("%d players found.", count);
        clif_displaymessage(fd, output);
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_whomap(const int fd, struct map_session_data *sd,
                      const char *, const char *message)
{
    struct map_session_data *pl_sd;
    int i, count;
    int pl_GM_level, GM_level;
    int map_id;
    char map_name[100];

    memset(map_name, '\0', sizeof(map_name));

    if (!message || !*message)
        map_id = sd->bl.m;
    else
    {
        sscanf(message, "%99s", map_name);
        if (strstr(map_name, ".gat") == NULL && strstr(map_name, ".afm") == NULL && strlen(map_name) < 13)   // 16 - 4 (.gat)
            strcat(map_name, ".gat");
        if ((map_id = map_mapname2mapid(map_name)) < 0)
            map_id = sd->bl.m;
    }

    count = 0;
    GM_level = pc_isGM(sd);
    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth)
        {
            pl_GM_level = pc_isGM(pl_sd);
            if (!
                ((battle_config.hide_GM_session
                  || bool(pl_sd->status.option & Option::HIDE))
                 && (pl_GM_level > GM_level)))
            {                   // you can look only lower or same level
                if (pl_sd->bl.m == map_id)
                {
                    std::string output;
                    if (pl_GM_level > 0)
                        output = STRPRINTF(
                                "Name: %s (GM:%d) | Location: %s %d %d",
                                pl_sd->status.name, pl_GM_level,
                                pl_sd->mapname, pl_sd->bl.x, pl_sd->bl.y);
                    else
                        output = STRPRINTF(
                                "Name: %s | Location: %s %d %d",
                                pl_sd->status.name, pl_sd->mapname,
                                pl_sd->bl.x, pl_sd->bl.y);
                    clif_displaymessage(fd, output);
                    count++;
                }
            }
        }
    }

    std::string output = STRPRINTF("%d players found in map '%s'.",
            count, map[map_id].name);
    clif_displaymessage(fd, output);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_whomapgroup(const int fd, struct map_session_data *sd,
                           const char *, const char *message)
{
    struct map_session_data *pl_sd;
    int i, count;
    int pl_GM_level, GM_level;
    int map_id = 0;
    char map_name[100];
    struct party *p;

    memset(map_name, '\0', sizeof(map_name));

    if (!message || !*message)
        map_id = sd->bl.m;
    else
    {
        sscanf(message, "%99s", map_name);
        if (strstr(map_name, ".gat") == NULL && strstr(map_name, ".afm") == NULL && strlen(map_name) < 13)   // 16 - 4 (.gat)
            strcat(map_name, ".gat");
        if ((map_id = map_mapname2mapid(map_name)) < 0)
            map_id = sd->bl.m;
    }

    count = 0;
    GM_level = pc_isGM(sd);
    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth)
        {
            pl_GM_level = pc_isGM(pl_sd);
            if (!
                ((battle_config.hide_GM_session
                  || bool(pl_sd->status.option & Option::HIDE))
                 && (pl_GM_level > GM_level)))
            {
                // you can look only lower or same level
                if (pl_sd->bl.m == map_id)
                {
                    p = party_search(pl_sd->status.party_id);
                    const char *temp0 = p ? p->name : "None";
                    std::string output;
                    if (pl_GM_level > 0)
                        output = STRPRINTF("Name: %s (GM:%d) | Party: '%s'",
                                pl_sd->status.name, pl_GM_level, temp0);
                    else
                        output = STRPRINTF("Name: %s | Party: '%s'",
                                pl_sd->status.name, temp0);
                    clif_displaymessage(fd, output);
                    count++;
                }
            }
        }
    }

    std::string output;
    if (count == 0)
        output = STRPRINTF("No player found in map '%s'.", map[map_id].name);
    else if (count == 1)
        output = STRPRINTF("1 player found in map '%s'.", map[map_id].name);
    else
    {
        output = STRPRINTF("%d players found in map '%s'.", count, map[map_id].name);
    }
    clif_displaymessage(fd, output);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_whogm(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    struct map_session_data *pl_sd;
    int i, j, count;
    int pl_GM_level, GM_level;
    char match_text[100];
    char player_name[24];
    struct party *p;

    memset(match_text, '\0', sizeof(match_text));
    memset(player_name, '\0', sizeof(player_name));

    if (sscanf(message, "%99[^\n]", match_text) < 1)
        strcpy(match_text, "");
    for (j = 0; match_text[j]; j++)
        match_text[j] = tolower(match_text[j]);

    count = 0;
    GM_level = pc_isGM(sd);
    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth)
        {
            pl_GM_level = pc_isGM(pl_sd);
            if (pl_GM_level > 0)
            {
                if (!
                    ((battle_config.hide_GM_session
                      || bool(pl_sd->status.option & Option::HIDE))
                     && (pl_GM_level > GM_level)))
                {
                    // you can look only lower or same level
                    memcpy(player_name, pl_sd->status.name, 24);
                    for (j = 0; player_name[j]; j++)
                        player_name[j] = tolower(player_name[j]);
                    if (strstr(player_name, match_text) != NULL)
                    {
                        // search with no case sensitive
                        std::string output;
                        output = STRPRINTF(
                                "Name: %s (GM:%d) | Location: %s %d %d",
                                pl_sd->status.name, pl_GM_level,
                                pl_sd->mapname, pl_sd->bl.x, pl_sd->bl.y);
                        clif_displaymessage(fd, output);
                        output = STRPRINTF(
                                "       BLvl: %d | Job: %s (Lvl: %d)",
                                pl_sd->status.base_level,
                                "Novice/Human",
                                pl_sd->status.job_level);
                        clif_displaymessage(fd, output);
                        p = party_search(pl_sd->status.party_id);
                        const char *temp0 = p ? p->name : "None";
                        output = STRPRINTF(
                                "       Party: '%s'",
                                temp0);
                        clif_displaymessage(fd, output);
                        count++;
                    }
                }
            }
        }
    }

    if (count == 0)
        clif_displaymessage(fd, "No GM found.");
    else if (count == 1)
        clif_displaymessage(fd, "1 GM found.");
    else
    {
        std::string output = STRPRINTF("%d GMs found.", count);
        clif_displaymessage(fd, output);
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_save(const int fd, struct map_session_data *sd,
                    const char *, const char *)
{
    nullpo_retr(-1, sd);

    pc_setsavepoint(sd, sd->mapname, sd->bl.x, sd->bl.y);
    pc_makesavestatus(sd);
    chrif_save(sd);
    clif_displaymessage(fd, "Character data respawn point saved.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_load(const int fd, struct map_session_data *sd,
                    const char *, const char *)
{
    int m;

    m = map_mapname2mapid(sd->status.save_point.map);
    if (m >= 0 && map[m].flag.nowarpto
        && battle_config.any_warp_GM_min_level > pc_isGM(sd))
    {
        clif_displaymessage(fd,
                             "You are not authorised to warp you to your save map.");
        return -1;
    }
    if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarp
        && battle_config.any_warp_GM_min_level > pc_isGM(sd))
    {
        clif_displaymessage(fd,
                             "You are not authorised to warp you from your actual map.");
        return -1;
    }

    pc_setpos(sd, sd->status.save_point.map, sd->status.save_point.x,
               sd->status.save_point.y, BeingRemoveWhy::GONE);
    clif_displaymessage(fd, "Warping to respawn point.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_speed(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    if (!message || !*message)
    {
        std::string output = STRPRINTF(
                "Please, enter a speed value (usage: @speed <%d-%d>).",
                static_cast<uint32_t>(MIN_WALK_SPEED.count()),
                static_cast<uint32_t>(MAX_WALK_SPEED.count()));
        clif_displaymessage(fd, output);
        return -1;
    }

    interval_t speed = static_cast<interval_t>(atoi(message));
    if (speed >= MIN_WALK_SPEED && speed <= MAX_WALK_SPEED)
    {
        sd->speed = speed;
        //sd->walktimer = x;
        //この文を追加 by れ
        clif_updatestatus(sd, SP::SPEED);
        clif_displaymessage(fd, "Speed changed.");
    }
    else
    {
        std::string output = STRPRINTF(
                "Please, enter a valid speed value (usage: @speed <%d-%d>).",
                static_cast<uint32_t>(MIN_WALK_SPEED.count()),
                static_cast<uint32_t>(MAX_WALK_SPEED.count()));
        clif_displaymessage(fd, output);
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_storage(const int fd, struct map_session_data *sd,
                       const char *, const char *)
{
    struct storage *stor;       //changes from Freya/Yor
    nullpo_retr(-1, sd);

    if (sd->state.storage_open)
    {
        clif_displaymessage(fd, "msg_table[250]");
        return -1;
    }

    if ((stor = account2storage2(sd->status.account_id)) != NULL
        && stor->storage_status == 1)
    {
        clif_displaymessage(fd, "msg_table[250]");
        return -1;
    }

    storage_storageopen(sd);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_option(const int fd, struct map_session_data *sd,
                      const char *, const char *message)
{
    int param1_ = 0, param2_ = 0, param3_ = 0;
    nullpo_retr(-1, sd);

    if (!message || !*message
        || sscanf(message, "%d %d %d", &param1_, &param2_, &param3_) < 1
        || param1_ < 0 || param2_ < 0 || param3_ < 0)
    {
        clif_displaymessage(fd,
                             "Please, enter at least a option (usage: @option <param1:0+> <param2:0+> <param3:0+>).");
        return -1;
    }

    Opt1 param1 = Opt1(param1_);
    Opt2 param2 = Opt2(param2_);
    Option param3 = Option(param3_);

    sd->opt1 = param1;
    sd->opt2 = param2;
    sd->status.option = param3;

    clif_changeoption(&sd->bl);
    pc_calcstatus(sd, 0);
    clif_displaymessage(fd, "Options changed.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_hide(const int fd, struct map_session_data *sd,
                    const char *, const char *)
{
    if (bool(sd->status.option & Option::HIDE))
    {
        sd->status.option &= ~Option::HIDE;
        clif_displaymessage(fd, "Invisible: Off.");    // Invisible: Off
    }
    else
    {
        sd->status.option |= Option::HIDE;
        clif_displaymessage(fd, "Invisible: On.");    // Invisible: On
    }
    clif_changeoption(&sd->bl);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_die(const int fd, struct map_session_data *sd,
                   const char *, const char *)
{
    pc_damage(NULL, sd, sd->status.hp + 1);
    clif_displaymessage(fd, "A pity! You've died.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_kill(const int fd, struct map_session_data *sd,
                    const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @kill <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can kill only lower or same level
            pc_damage(NULL, pl_sd, pl_sd->status.hp + 1);
            clif_displaymessage(fd, "Character killed.");
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_alive(const int fd, struct map_session_data *sd,
                     const char *, const char *)
{
    sd->status.hp = sd->status.max_hp;
    sd->status.sp = sd->status.max_sp;
    pc_setstand(sd);
    if (static_cast<interval_t>(battle_config.pc_invincible_time) > interval_t::zero())
        pc_setinvincibletimer(sd, static_cast<interval_t>(battle_config.pc_invincible_time));
    clif_updatestatus(sd, SP::HP);
    clif_updatestatus(sd, SP::SP);
    clif_resurrection(&sd->bl, 1);
    clif_displaymessage(fd, "You've been revived! It's a miracle!");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_kami(const int fd, struct map_session_data *,
                    const char *, const char *message)
{
    if (!message || !*message)
    {
        clif_displaymessage(fd,
                             "Please, enter a message (usage: @kami <message>).");
        return -1;
    }

    intif_GMmessage(message, 0);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_heal(const int fd, struct map_session_data *sd,
                    const char *, const char *message)
{
    int hp = 0, sp = 0;        // [Valaris] thanks to fov

    sscanf(message, "%d %d", &hp, &sp);

    if (hp == 0 && sp == 0)
    {
        hp = sd->status.max_hp - sd->status.hp;
        sp = sd->status.max_sp - sd->status.sp;
    }
    else
    {
        if (hp > 0 && (hp > sd->status.max_hp || hp > (sd->status.max_hp - sd->status.hp))) // fix positiv overflow
            hp = sd->status.max_hp - sd->status.hp;
        else if (hp < 0 && (hp < -sd->status.max_hp || hp < (1 - sd->status.hp)))   // fix negativ overflow
            hp = 1 - sd->status.hp;
        if (sp > 0 && (sp > sd->status.max_sp || sp > (sd->status.max_sp - sd->status.sp))) // fix positiv overflow
            sp = sd->status.max_sp - sd->status.sp;
        else if (sp < 0 && (sp < -sd->status.max_sp || sp < (1 - sd->status.sp)))   // fix negativ overflow
            sp = 1 - sd->status.sp;
    }

    if (hp < 0)            // display like damage
        clif_damage(&sd->bl, &sd->bl, gettick(), interval_t::zero(), interval_t::zero(), -hp, 0, DamageType::RETURNED, 0);

    if (hp != 0 || sp != 0)
    {
        pc_heal(sd, hp, sp);
        if (hp >= 0 && sp >= 0)
            clif_displaymessage(fd, "HP, SP recovered.");
        else
            clif_displaymessage(fd, "HP or/and SP modified.");
    }
    else
    {
        clif_displaymessage(fd, "HP and SP are already with the good value.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @item command (usage: @item <name/id_of_item> <quantity>)
 *------------------------------------------
 */
int atcommand_item(const int fd, struct map_session_data *sd,
                    const char *, const char *message)
{
    char item_name[100];
    int number = 0, item_id;
    struct item item_tmp;
    struct item_data *item_data;
    int get_count, i;

    memset(item_name, '\0', sizeof(item_name));

    if (!message || !*message
        || sscanf(message, "%99s %d", item_name, &number) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter an item name/id (usage: @item <item name or ID> [quantity]).");
        return -1;
    }

    if (number <= 0)
        number = 1;

    item_id = 0;
    if ((item_data = itemdb_searchname(item_name)) != NULL ||
        (item_data = itemdb_exists(atoi(item_name))) != NULL)
        item_id = item_data->nameid;

    if (item_id >= 500)
    {
        get_count = number;
        if (item_data->type == ItemType::WEAPON
            || item_data->type == ItemType::ARMOR
            || item_data->type == ItemType::_7
            || item_data->type == ItemType::_8)
        {
            get_count = 1;
        }
        for (i = 0; i < number; i += get_count)
        {
            memset(&item_tmp, 0, sizeof(item_tmp));
            item_tmp.nameid = item_id;
            item_tmp.identify = 1;
            PickupFail flag;
            if ((flag = pc_additem(sd, &item_tmp, get_count))
                != PickupFail::OKAY)
                clif_additem(sd, 0, 0, flag);
        }
        clif_displaymessage(fd, "Item created.");
    }
    else
    {
        clif_displaymessage(fd, "Invalid item ID or name.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_itemreset(const int fd, struct map_session_data *sd,
                         const char *, const char *)
{
    int i;

    for (i = 0; i < MAX_INVENTORY; i++)
    {
        if (sd->status.inventory[i].amount
            && sd->status.inventory[i].equip == EPOS::ZERO)
            pc_delitem(sd, i, sd->status.inventory[i].amount, 0);
    }
    clif_displaymessage(fd, "All of your items have been removed.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_itemcheck(const int, struct map_session_data *sd,
                         const char *, const char *)
{
    pc_checkitem(sd);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_baselevelup(const int fd, struct map_session_data *sd,
                           const char *, const char *message)
{
    int level, i;

    if (!message || !*message || (level = atoi(message)) == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a level adjustement (usage: @blvl <number of levels>).");
        return -1;
    }

    if (level > 0)
    {
        if (sd->status.base_level == battle_config.maximum_level)
        {                       // check for max level by Valaris
            clif_displaymessage(fd, "Base level can't go any higher.");
            return -1;
        }                       // End Addition
        if (level > battle_config.maximum_level || level > (battle_config.maximum_level - sd->status.base_level))   // fix positiv overflow
            level = battle_config.maximum_level - sd->status.base_level;
        for (i = 1; i <= level; i++)
            sd->status.status_point += (sd->status.base_level + i + 14) / 4;
        sd->status.base_level += level;
        clif_updatestatus(sd, SP::BASELEVEL);
        clif_updatestatus(sd, SP::NEXTBASEEXP);
        clif_updatestatus(sd, SP::STATUSPOINT);
        pc_calcstatus(sd, 0);
        pc_heal(sd, sd->status.max_hp, sd->status.max_sp);
        clif_misceffect(&sd->bl, 0);
        clif_displaymessage(fd, "Base level raised.");
    }
    else
    {
        if (sd->status.base_level == 1)
        {
            clif_displaymessage(fd, "Base level can't go any lower.");
            return -1;
        }
        if (level < -battle_config.maximum_level || level < (1 - sd->status.base_level))    // fix negativ overflow
            level = 1 - sd->status.base_level;
        if (sd->status.status_point > 0)
        {
            for (i = 0; i > level; i--)
                sd->status.status_point -=
                    (sd->status.base_level + i + 14) / 4;
            if (sd->status.status_point < 0)
                sd->status.status_point = 0;
            clif_updatestatus(sd, SP::STATUSPOINT);
        }                       // to add: remove status points from stats
        sd->status.base_level += level;
        clif_updatestatus(sd, SP::BASELEVEL);
        clif_updatestatus(sd, SP::NEXTBASEEXP);
        pc_calcstatus(sd, 0);
        clif_displaymessage(fd, "Base level lowered.");
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
// TODO: merge this with pc_setparam(SP::JOBLEVEL)
// then fix the funny 50 and/or 10 limitation.
int atcommand_joblevelup(const int fd, struct map_session_data *sd,
                          const char *, const char *message)
{
    int up_level = 50, level;

    if (!message || !*message || (level = atoi(message)) == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a level adjustement (usage: @jlvl <number of levels>).");
        return -1;
    }

    up_level -= 40;

    if (level > 0)
    {
        if (sd->status.job_level == up_level)
        {
            clif_displaymessage(fd, "Job level can't go any higher.");
            return -1;
        }
        if (level > up_level || level > (up_level - sd->status.job_level))  // fix positiv overflow
            level = up_level - sd->status.job_level;
        sd->status.job_level += level;
        clif_updatestatus(sd, SP::JOBLEVEL);
        clif_updatestatus(sd, SP::NEXTJOBEXP);
        sd->status.skill_point += level;
        clif_updatestatus(sd, SP::SKILLPOINT);
        pc_calcstatus(sd, 0);
        clif_misceffect(&sd->bl, 1);
        clif_displaymessage(fd, "Job level raised.");
    }
    else
    {
        if (sd->status.job_level == 1)
        {
            clif_displaymessage(fd, "Job level can't go any lower.");
            return -1;
        }
        if (level < -up_level || level < (1 - sd->status.job_level))    // fix negativ overflow
            level = 1 - sd->status.job_level;
        sd->status.job_level += level;
        clif_updatestatus(sd, SP::JOBLEVEL);
        clif_updatestatus(sd, SP::NEXTJOBEXP);
        if (sd->status.skill_point > 0)
        {
            sd->status.skill_point += level;
            if (sd->status.skill_point < 0)
                sd->status.skill_point = 0;
            clif_updatestatus(sd, SP::SKILLPOINT);
        }                       // to add: remove status points from skills
        pc_calcstatus(sd, 0);
        clif_displaymessage(fd, "Job level lowered.");
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_help(const int fd, struct map_session_data *sd,
                    const char *, const char *)
{
    char buf[2048], w1[2048], w2[2048];
    int i, gm_level;
    FILE *fp;

    memset(buf, '\0', sizeof(buf));

    if ((fp = fopen_(help_txt, "r")) != NULL)
    {
        clif_displaymessage(fd, "Help commands:");
        gm_level = pc_isGM(sd);
        while (fgets(buf, sizeof(buf) - 1, fp) != NULL)
        {
            if (buf[0] == '/' && buf[1] == '/')
                continue;
            for (i = 0; buf[i] != '\0'; i++)
            {
                if (buf[i] == '\r' || buf[i] == '\n')
                {
                    buf[i] = '\0';
                    break;
                }
            }
            if (sscanf(buf, "%2047[^:]:%2047[^\n]", w1, w2) < 2)
                clif_displaymessage(fd, buf);
            else if (gm_level >= atoi(w1))
                clif_displaymessage(fd, w2);
        }
        fclose_(fp);
    }
    else
    {
        clif_displaymessage(fd, "File help.txt not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_gm(const int fd, struct map_session_data *sd,
                  const char *, const char *message)
{
    char password[100];

    memset(password, '\0', sizeof(password));

    if (!message || !*message || sscanf(message, "%99[^\n]", password) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a password (usage: @gm <password>).");
        return -1;
    }

    if (pc_isGM(sd))
    {                           // a GM can not use this function. only a normal player (become gm is not for gm!)
        clif_displaymessage(fd, "You already have some GM powers.");
        return -1;
    }
    else
        chrif_changegm(sd->status.account_id, password,
                        strlen(password) + 1);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_pvpoff(const int fd, struct map_session_data *sd,
                      const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;

    if (battle_config.pk_mode)
    {                           //disable command if server is in PK mode [Valaris]
        clif_displaymessage(fd, "This option cannot be used in PK Mode.");
        return -1;
    }

    if (map[sd->bl.m].flag.pvp)
    {
        map[sd->bl.m].flag.pvp = 0;
        for (i = 0; i < fd_max; i++)
        {                       //人数分ループ
            if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
                && pl_sd->state.auth)
            {
                if (sd->bl.m == pl_sd->bl.m)
                {
                    if (pl_sd->pvp_timer)
                    {
                        delete_timer(pl_sd->pvp_timer);
                        pl_sd->pvp_timer = nullptr;
                    }
                }
            }
        }
        clif_displaymessage(fd, "PvP: Off.");
    }
    else
    {
        clif_displaymessage(fd, "PvP is already Off.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_pvpon(const int fd, struct map_session_data *sd,
                     const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;

    if (battle_config.pk_mode)
    {                           //disable command if server is in PK mode [Valaris]
        clif_displaymessage(fd, "This option cannot be used in PK Mode.");
        return -1;
    }

    if (!map[sd->bl.m].flag.pvp && !map[sd->bl.m].flag.nopvp)
    {
        map[sd->bl.m].flag.pvp = 1;
        for (i = 0; i < fd_max; i++)
        {
            if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
                && pl_sd->state.auth)
            {
                if (sd->bl.m == pl_sd->bl.m && !pl_sd->pvp_timer)
                {
                    pl_sd->pvp_timer = add_timer(gettick() + std::chrono::milliseconds(200),
                            std::bind(pc_calc_pvprank_timer, ph::_1, ph::_2, pl_sd->bl.id));
                    pl_sd->pvp_rank = 0;
                    pl_sd->pvp_lastusers = 0;
                    pl_sd->pvp_point = 5;
                }
            }
        }
        clif_displaymessage(fd, "PvP: On.");
    }
    else
    {
        clif_displaymessage(fd, "PvP is already On.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_model(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    int hair_style = 0, hair_color = 0, cloth_color = 0;

    if (!message || !*message
        || sscanf(message, "%d %d %d", &hair_style, &hair_color,
                   &cloth_color) < 1)
    {
        std::string output = STRPRINTF(
                "Please, enter at least a value (usage: @model <hair ID: %d-%d> <hair color: %d-%d> <clothes color: %d-%d>).",
                MIN_HAIR_STYLE, MAX_HAIR_STYLE,
                MIN_HAIR_COLOR, MAX_HAIR_COLOR,
                MIN_CLOTH_COLOR, MAX_CLOTH_COLOR);
        clif_displaymessage(fd, output);
        return -1;
    }

    if (hair_style >= MIN_HAIR_STYLE && hair_style <= MAX_HAIR_STYLE &&
        hair_color >= MIN_HAIR_COLOR && hair_color <= MAX_HAIR_COLOR &&
        cloth_color >= MIN_CLOTH_COLOR && cloth_color <= MAX_CLOTH_COLOR)
    {
        {
            pc_changelook(sd, LOOK::HAIR, hair_style);
            pc_changelook(sd, LOOK::HAIR_COLOR, hair_color);
            pc_changelook(sd, LOOK::CLOTHES_COLOR, cloth_color);
            clif_displaymessage(fd, "Appearence changed.");
        }
    }
    else
    {
        clif_displaymessage(fd, "An invalid number was specified.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @dye && @ccolor
 *------------------------------------------
 */
int atcommand_dye(const int fd, struct map_session_data *sd,
                   const char *, const char *message)
{
    int cloth_color = 0;

    if (!message || !*message || sscanf(message, "%d", &cloth_color) < 1)
    {
        std::string output = STRPRINTF(
                "Please, enter a clothes color (usage: @dye/@ccolor <clothes color: %d-%d>).",
                MIN_CLOTH_COLOR, MAX_CLOTH_COLOR);
        clif_displaymessage(fd, output);
        return -1;
    }

    if (cloth_color >= MIN_CLOTH_COLOR && cloth_color <= MAX_CLOTH_COLOR)
    {
        {
            pc_changelook(sd, LOOK::CLOTHES_COLOR, cloth_color);
            clif_displaymessage(fd, "Appearence changed.");
        }
    }
    else
    {
        clif_displaymessage(fd, "An invalid number was specified.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @hairstyle && @hstyle
 *------------------------------------------
 */
int atcommand_hair_style(const int fd, struct map_session_data *sd,
                          const char *, const char *message)
{
    int hair_style = 0;

    if (!message || !*message || sscanf(message, "%d", &hair_style) < 1)
    {
        std::string output = STRPRINTF(
                "Please, enter a hair style (usage: @hairstyle/@hstyle <hair ID: %d-%d>).",
                MIN_HAIR_STYLE, MAX_HAIR_STYLE);
        clif_displaymessage(fd, output);
        return -1;
    }

    if (hair_style >= MIN_HAIR_STYLE && hair_style <= MAX_HAIR_STYLE)
    {
        {
            pc_changelook(sd, LOOK::HAIR, hair_style);
            clif_displaymessage(fd, "Appearence changed.");
        }
    }
    else
    {
        clif_displaymessage(fd, "An invalid number was specified.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @haircolor && @hcolor
 *------------------------------------------
 */
int atcommand_hair_color(const int fd, struct map_session_data *sd,
                          const char *, const char *message)
{
    int hair_color = 0;

    if (!message || !*message || sscanf(message, "%d", &hair_color) < 1)
    {
        std::string output = STRPRINTF(
                "Please, enter a hair color (usage: @haircolor/@hcolor <hair color: %d-%d>).",
                MIN_HAIR_COLOR, MAX_HAIR_COLOR);
        clif_displaymessage(fd, output);
        return -1;
    }

    if (hair_color >= MIN_HAIR_COLOR && hair_color <= MAX_HAIR_COLOR)
    {
        {
            pc_changelook(sd, LOOK::HAIR_COLOR, hair_color);
            clif_displaymessage(fd, "Appearence changed.");
        }
    }
    else
    {
        clif_displaymessage(fd, "An invalid number was specified.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_spawn(const int fd, struct map_session_data *sd,
                     const char *command, const char *message)
{
    char monster[100];
    int mob_id;
    int number = 0;
    int x = 0, y = 0;
    int count;
    int i, j, k;
    int mx, my, range;

    memset(monster, '\0', sizeof(monster));

    if (!message || !*message
        || sscanf(message, "%99s %d %d %d", monster, &number, &x, &y) < 1)
    {
        clif_displaymessage(fd, "Give a monster name/id please.");
        return -1;
    }

    // If monster identifier/name argument is a name
    if ((mob_id = mobdb_searchname(monster)) == 0) // check name first (to avoid possible name begining by a number)
        mob_id = mobdb_checkid(atoi(monster));

    if (mob_id == 0)
    {
        clif_displaymessage(fd, "Invalid monster ID or name.");
        return -1;
    }

    if (mob_id == 1288)
    {
        clif_displaymessage(fd, "Cannot spawn emperium.");
        return -1;
    }

    if (number <= 0)
        number = 1;

    // If value of atcommand_spawn_quantity_limit directive is greater than or equal to 1 and quantity of monsters is greater than value of the directive
    if (battle_config.atc_spawn_quantity_limit >= 1
        && number > battle_config.atc_spawn_quantity_limit)
        number = battle_config.atc_spawn_quantity_limit;

    if (battle_config.etc_log)
        PRINTF("%s monster='%s' id=%d count=%d (%d,%d)\n", command, monster,
                mob_id, number, x, y);

    count = 0;
    range = sqrt(number) / 2;
    range = range * 2 + 5;      // calculation of an odd number (+ 4 area around)
    for (i = 0; i < number; i++)
    {
        j = 0;
        k = 0;
        while (j++ < 8 && k == 0)
        {
            // try 8 times to spawn the monster (needed for close area)
            if (x <= 0)
                mx = sd->bl.x + random_::in(-range / 2, range / 2 );
            else
                mx = x;
            if (y <= 0)
                my = sd->bl.y + random_::in(-range / 2, range / 2);
            else
                my = y;
            k = mob_once_spawn(sd, "this", mx, my, "", mob_id, 1, "");
        }
        count += (k != 0) ? 1 : 0;
    }

    if (count != 0)
        if (number == count)
            clif_displaymessage(fd, "All monster summoned!");
        else
        {
            std::string output = STRPRINTF("%d monster(s) summoned!",
                    count);
            clif_displaymessage(fd, output);
        }
    else
    {
        clif_displaymessage(fd, "Invalid monster ID or name.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
static
void atcommand_killmonster_sub(const int fd, struct map_session_data *sd,
                                const char *message, const int drop)
{
    int map_id;
    char map_name[100];

    memset(map_name, '\0', sizeof(map_name));

    if (!message || !*message || sscanf(message, "%99s", map_name) < 1)
        map_id = sd->bl.m;
    else
    {
        if (strstr(map_name, ".gat") == NULL && strstr(map_name, ".afm") == NULL && strlen(map_name) < 13)   // 16 - 4 (.gat)
            strcat(map_name, ".gat");
        if ((map_id = map_mapname2mapid(map_name)) < 0)
            map_id = sd->bl.m;
    }

    map_foreachinarea(std::bind(atkillmonster_sub, ph::_1, drop), map_id, 0, 0, map[map_id].xs,
                       map[map_id].ys, BL::MOB);

    clif_displaymessage(fd, "All monsters killed!");

    return;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_killmonster(const int fd, struct map_session_data *sd,
                           const char *, const char *message)
{
    atcommand_killmonster_sub(fd, sd, message, 1);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
static
void atlist_nearby_sub(struct block_list *bl, int fd)
{
    nullpo_retv(bl);

    std::string buf = STRPRINTF(" - \"%s\"",
            ((struct map_session_data *) bl)->status.name);
    clif_displaymessage(fd, buf);
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_list_nearby(const int fd, struct map_session_data *sd,
                           const char *, const char *)
{
    clif_displaymessage(fd, "Nearby players:");
    map_foreachinarea(std::bind(atlist_nearby_sub, ph::_1, fd),
            sd->bl.m, sd->bl.x - 1, sd->bl.y - 1,
            sd->bl.x + 1, sd->bl.x + 1, BL::PC);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_killmonster2(const int fd, struct map_session_data *sd,
                            const char *, const char *message)
{
    atcommand_killmonster_sub(fd, sd, message, 0);

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_gat(const int fd, struct map_session_data *sd,
                   const char *, const char *)
{
    int y;

    for (y = 2; y >= -2; y--)
    {
        std::string output = STRPRINTF(
                "%s (x= %d, y= %d) %02X %02X %02X %02X %02X",
                map[sd->bl.m].name, sd->bl.x - 2, sd->bl.y + y,
                map_getcell(sd->bl.m, sd->bl.x - 2, sd->bl.y + y),
                map_getcell(sd->bl.m, sd->bl.x - 1, sd->bl.y + y),
                map_getcell(sd->bl.m, sd->bl.x, sd->bl.y + y),
                map_getcell(sd->bl.m, sd->bl.x + 1, sd->bl.y + y),
                map_getcell(sd->bl.m, sd->bl.x + 2, sd->bl.y + y));
        clif_displaymessage(fd, output);
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_packet(const int fd, struct map_session_data *sd,
                      const char *, const char *message)
{
    int type = 0, flag = 0;

    if (!message || !*message || sscanf(message, "%d %d", &type, &flag) < 2)
    {
        clif_displaymessage(fd,
                             "Please, enter a status type/flag (usage: @packet <status type> <flag>).");
        return -1;
    }

    clif_status_change(&sd->bl, StatusChange(type), flag);

    return 0;
}

/*==========================================
 * @stpoint (Rewritten by [Yor])
 *------------------------------------------
 */
int atcommand_statuspoint(const int fd, struct map_session_data *sd,
                           const char *, const char *message)
{
    int point, new_status_point;

    if (!message || !*message || (point = atoi(message)) == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a number (usage: @stpoint <number of points>).");
        return -1;
    }

    new_status_point = (int) sd->status.status_point + point;
    if (point > 0 && (point > 0x7FFF || new_status_point > 0x7FFF)) // fix positiv overflow
        new_status_point = 0x7FFF;
    else if (point < 0 && (point < -0x7FFF || new_status_point < 0))    // fix negativ overflow
        new_status_point = 0;

    if (new_status_point != (int) sd->status.status_point)
    {
        sd->status.status_point = (short) new_status_point;
        clif_updatestatus(sd, SP::STATUSPOINT);
        clif_displaymessage(fd, "Number of status points changed!");
    }
    else
    {
        if (point < 0)
            clif_displaymessage(fd, "Impossible to decrease the number/value.");
        else
            clif_displaymessage(fd, "Impossible to increase the number/value.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @skpoint (Rewritten by [Yor])
 *------------------------------------------
 */
int atcommand_skillpoint(const int fd, struct map_session_data *sd,
                          const char *, const char *message)
{
    int point, new_skill_point;

    if (!message || !*message || (point = atoi(message)) == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a number (usage: @skpoint <number of points>).");
        return -1;
    }

    new_skill_point = (int) sd->status.skill_point + point;
    if (point > 0 && (point > 0x7FFF || new_skill_point > 0x7FFF))  // fix positiv overflow
        new_skill_point = 0x7FFF;
    else if (point < 0 && (point < -0x7FFF || new_skill_point < 0)) // fix negativ overflow
        new_skill_point = 0;

    if (new_skill_point != (int) sd->status.skill_point)
    {
        sd->status.skill_point = (short) new_skill_point;
        clif_updatestatus(sd, SP::SKILLPOINT);
        clif_displaymessage(fd, "Number of skill points changed!");
    }
    else
    {
        if (point < 0)
            clif_displaymessage(fd, "Impossible to decrease the number/value.");
        else
            clif_displaymessage(fd, "Impossible to increase the number/value.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @zeny (Rewritten by [Yor])
 *------------------------------------------
 */
int atcommand_zeny(const int fd, struct map_session_data *sd,
                    const char *, const char *message)
{
    int zeny, new_zeny;

    if (!message || !*message || (zeny = atoi(message)) == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter an amount (usage: @zeny <amount>).");
        return -1;
    }

    new_zeny = sd->status.zeny + zeny;
    if (zeny > 0 && (zeny > MAX_ZENY || new_zeny > MAX_ZENY))   // fix positiv overflow
        new_zeny = MAX_ZENY;
    else if (zeny < 0 && (zeny < -MAX_ZENY || new_zeny < 0))    // fix negativ overflow
        new_zeny = 0;

    if (new_zeny != sd->status.zeny)
    {
        sd->status.zeny = new_zeny;
        clif_updatestatus(sd, SP::ZENY);
        clif_displaymessage(fd, "Number of zenys changed!");
    }
    else
    {
        if (zeny < 0)
            clif_displaymessage(fd, "Impossible to decrease the number/value.");
        else
            clif_displaymessage(fd, "Impossible to increase the number/value.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
template<ATTR attr>
int atcommand_param(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    int value = 0, new_value;

    if (!message || !*message || sscanf(message, "%d", &value) < 1
        || value == 0)
    {

        // there was a clang bug here
        // fortunately, STRPRINTF was not actually needed
        clif_displaymessage(fd,
                "Please, enter a valid value (usage: @str,@agi,@vit,@int,@dex,@luk <+/-adjustement>).");
        return -1;
    }

    new_value = (int) sd->status.attrs[attr] + value;
    if (value > 0 && (value > battle_config.max_parameter || new_value > battle_config.max_parameter))  // fix positiv overflow
        new_value = battle_config.max_parameter;
    else if (value < 0 && (value < -battle_config.max_parameter || new_value < 1))  // fix negativ overflow
        new_value = 1;

    if (new_value != sd->status.attrs[attr])
    {
        sd->status.attrs[attr] = new_value;
        clif_updatestatus(sd, attr_to_sp(attr));
        clif_updatestatus(sd, attr_to_usp(attr));
        pc_calcstatus(sd, 0);
        clif_displaymessage(fd, "Stat changed.");
    }
    else
    {
        if (value < 0)
            clif_displaymessage(fd, "Impossible to decrease the number/value.");
        else
            clif_displaymessage(fd, "Impossible to increase the number/value.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
//** Stat all by fritz (rewritten by [Yor])
int atcommand_all_stats(const int fd, struct map_session_data *sd,
                         const char *, const char *message)
{
    int count, value = 0, new_value;

    if (!message || !*message || sscanf(message, "%d", &value) < 1
        || value == 0)
        value = battle_config.max_parameter;

    count = 0;
    for (ATTR attr : ATTRs)
    {
        new_value = sd->status.attrs[attr] + value;
        if (value > 0 && (value > battle_config.max_parameter || new_value > battle_config.max_parameter))  // fix positiv overflow
            new_value = battle_config.max_parameter;
        else if (value < 0 && (value < -battle_config.max_parameter || new_value < 1))  // fix negativ overflow
            new_value = 1;

        if (new_value != sd->status.attrs[attr])
        {
            sd->status.attrs[attr] = new_value;
            clif_updatestatus(sd, attr_to_sp(attr));
            clif_updatestatus(sd, attr_to_usp(attr));
            pc_calcstatus(sd, 0);
            count++;
        }
    }

    if (count > 0)              // if at least 1 stat modified
        clif_displaymessage(fd, "All stats changed!");
    else
    {
        if (value < 0)
            clif_displaymessage(fd, "Impossible to decrease a stat.");
        else
            clif_displaymessage(fd, "Impossible to increase a stat.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_recall(const int fd, struct map_session_data *sd,
                      const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @recall <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can recall only lower or same level
            if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarpto
                && battle_config.any_warp_GM_min_level > pc_isGM(sd))
            {
                clif_displaymessage(fd,
                                     "You are not authorised to warp somenone to your actual map.");
                return -1;
            }
            if (pl_sd->bl.m >= 0 && map[pl_sd->bl.m].flag.nowarp
                && battle_config.any_warp_GM_min_level > pc_isGM(sd))
            {
                clif_displaymessage(fd,
                                     "You are not authorised to warp this player from its actual map.");
                return -1;
            }
            pc_setpos(pl_sd, sd->mapname, sd->bl.x, sd->bl.y, BeingRemoveWhy::QUIT);
            std::string output = STRPRINTF("%s recalled!", character);
            clif_displaymessage(fd, output);
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_revive(const int fd, struct map_session_data *sd,
                      const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @revive <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        pl_sd->status.hp = pl_sd->status.max_hp;
        pc_setstand(pl_sd);
        if (static_cast<interval_t>(battle_config.pc_invincible_time) > interval_t::zero())
            pc_setinvincibletimer(sd, static_cast<interval_t>(battle_config.pc_invincible_time));
        clif_updatestatus(pl_sd, SP::HP);
        clif_updatestatus(pl_sd, SP::SP);
        clif_resurrection(&pl_sd->bl, 1);
        clif_displaymessage(fd, "Character revived.");
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_character_stats(const int fd, struct map_session_data *,
                               const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charstats <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        std::string output;
        output = STRPRINTF("'%s' stats:", pl_sd->status.name);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Base Level - %d", pl_sd->status.base_level),
        clif_displaymessage(fd, output);
        output = STRPRINTF("Job - Novice/Human (level %d)", pl_sd->status.job_level);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Hp - %d", pl_sd->status.hp);
        clif_displaymessage(fd, output);
        output = STRPRINTF("MaxHp - %d", pl_sd->status.max_hp);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Sp - %d", pl_sd->status.sp);
        clif_displaymessage(fd, output);
        output = STRPRINTF("MaxSp - %d", pl_sd->status.max_sp);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Str - %3d", pl_sd->status.attrs[ATTR::STR]);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Agi - %3d", pl_sd->status.attrs[ATTR::AGI]);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Vit - %3d", pl_sd->status.attrs[ATTR::VIT]);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Int - %3d", pl_sd->status.attrs[ATTR::INT]);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Dex - %3d", pl_sd->status.attrs[ATTR::DEX]);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Luk - %3d", pl_sd->status.attrs[ATTR::LUK]);
        clif_displaymessage(fd, output);
        output = STRPRINTF("Zeny - %d", pl_sd->status.zeny);
        clif_displaymessage(fd, output);
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
//** Character Stats All by fritz
int atcommand_character_stats_all(const int fd, struct map_session_data *,
                                   const char *, const char *)
{
    int i;
    int count;
    struct map_session_data *pl_sd;

    count = 0;
    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth)
        {
            std::string gmlevel;
            if (pc_isGM(pl_sd) > 0)
                gmlevel = STRPRINTF("| GM Lvl: %d", pc_isGM(pl_sd));
            else
                gmlevel = " ";

            std::string output;
            output = STRPRINTF(
                    "Name: %s | BLvl: %d | Job: Novice/Human (Lvl: %d) | HP: %d/%d | SP: %d/%d",
                    pl_sd->status.name, pl_sd->status.base_level,
                    pl_sd->status.job_level,
                    pl_sd->status.hp, pl_sd->status.max_hp,
                    pl_sd->status.sp, pl_sd->status.max_sp);
            clif_displaymessage(fd, output);
            output = STRPRINTF("STR: %d | AGI: %d | VIT: %d | INT: %d | DEX: %d | LUK: %d | Zeny: %d %s",
                     pl_sd->status.attrs[ATTR::STR],
                     pl_sd->status.attrs[ATTR::AGI],
                     pl_sd->status.attrs[ATTR::VIT],
                     pl_sd->status.attrs[ATTR::INT],
                     pl_sd->status.attrs[ATTR::DEX],
                     pl_sd->status.attrs[ATTR::LUK],
                     pl_sd->status.zeny,
                     gmlevel);
            clif_displaymessage(fd, output);
            clif_displaymessage(fd, "--------");
            count++;
        }
    }

    if (count == 0)
        clif_displaymessage(fd, "No player found.");
    else if (count == 1)
        clif_displaymessage(fd, "1 player found.");
    else
    {
        std::string output = STRPRINTF("%d players found.", count);
        clif_displaymessage(fd, output);
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_character_option(const int fd, struct map_session_data *sd,
                                const char *, const char *message)
{
    char character[100];
    int opt1_ = 0, opt2_ = 0, opt3_ = 0;
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %d %d %99[^\n]", &opt1_, &opt2_, &opt3_,
                   character) < 4 || opt1_ < 0 || opt2_ < 0 || opt3_ < 0)
    {
        clif_displaymessage(fd,
                             "Please, enter valid options and a player name (usage: @charoption <param1> <param2> <param3> <charname>).");
        return -1;
    }

    Opt1 opt1 = Opt1(opt1_);
    Opt2 opt2 = Opt2(opt2_);
    Option opt3 = Option(opt3_);

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {
            // you can change option only to lower or same level
            pl_sd->opt1 = opt1;
            pl_sd->opt2 = opt2;
            pl_sd->status.option = opt3;

            clif_changeoption(&pl_sd->bl);
            pc_calcstatus(pl_sd, 0);
            clif_displaymessage(fd, "Character's options changed.");
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * charchangesex command (usage: charchangesex <player_name>)
 *------------------------------------------
 */
int atcommand_char_change_sex(const int fd, struct map_session_data *sd,
                               const char *, const char *message)
{
    char character[100];

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charchangesex <name>).");
        return -1;
    }

    // check player name
    if (strlen(character) < 4)
    {
        clif_displaymessage(fd, "Sorry, but a player name have at least 4 characters.");
        return -1;
    }
    else if (strlen(character) > 23)
    {
        clif_displaymessage(fd, "Sorry, but a player name have 23 characters maximum.");
        return -1;
    }
    else
    {
        chrif_char_ask_name(sd->status.account_id, character, 5, 0, 0, 0, 0, 0, 0);    // type: 5 - changesex
        clif_displaymessage(fd, "Character name sends to char-server to ask it.");
    }

    return 0;
}

/*==========================================
 * charblock command (usage: charblock <player_name>)
 * This command do a definitiv ban on a player
 *------------------------------------------
 */
int atcommand_char_block(const int fd, struct map_session_data *sd,
                          const char *, const char *message)
{
    char character[100];

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @block <name>).");
        return -1;
    }

    // check player name
    if (strlen(character) < 4)
    {
        clif_displaymessage(fd, "Sorry, but a player name have at least 4 characters.");
        return -1;
    }
    else if (strlen(character) > 23)
    {
        clif_displaymessage(fd, "Sorry, but a player name have 23 characters maximum.");
        return -1;
    }
    else
    {
        chrif_char_ask_name(sd->status.account_id, character, 1, 0, 0, 0, 0, 0, 0);    // type: 1 - block
        clif_displaymessage(fd, "Character name sends to char-server to ask it.");
    }

    return 0;
}

/*==========================================
 * charban command (usage: charban <time> <player_name>)
 * This command do a limited ban on a player
 * Time is done as follows:
 *   Adjustment value (-1, 1, +1, etc...)
 *   Modified element:
 *     a or y: year
 *     m:  month
 *     j or d: day
 *     h:  hour
 *     mn: minute
 *     s:  second
 * <example> @ban +1m-2mn1s-6y test_player
 *           this example adds 1 month and 1 second, and substracts 2 minutes and 6 years at the same time.
 *------------------------------------------
 */
int atcommand_char_ban(const int fd, struct map_session_data *sd,
                        const char *, const char *message)
{
    char modif[100], character[100];
    char *modif_p;
    int year, month, day, hour, minute, second, value;

    memset(modif, '\0', sizeof(modif));
    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%s %99[^\n]", modif, character) < 2)
    {
        clif_displaymessage(fd,
                             "Please, enter ban time and a player name (usage: @charban/@ban/@banish/@charbanish <time> <name>).");
        return -1;
    }

    modif[sizeof(modif) - 1] = '\0';
    character[sizeof(character) - 1] = '\0';

    modif_p = modif;
    year = month = day = hour = minute = second = 0;
    while (modif_p[0] != '\0')
    {
        value = atoi(modif_p);
        if (value == 0)
            modif_p++;
        else
        {
            if (modif_p[0] == '-' || modif_p[0] == '+')
                modif_p++;
            while (modif_p[0] >= '0' && modif_p[0] <= '9')
                modif_p++;
            if (modif_p[0] == 's')
            {
                second = value;
                modif_p++;
            }
            else if (modif_p[0] == 'm' && modif_p[1] == 'n')
            {
                minute = value;
                modif_p = modif_p + 2;
            }
            else if (modif_p[0] == 'h')
            {
                hour = value;
                modif_p++;
            }
            else if (modif_p[0] == 'd' || modif_p[0] == 'j')
            {
                day = value;
                modif_p++;
            }
            else if (modif_p[0] == 'm')
            {
                month = value;
                modif_p++;
            }
            else if (modif_p[0] == 'y' || modif_p[0] == 'a')
            {
                year = value;
                modif_p++;
            }
            else if (modif_p[0] != '\0')
            {
                modif_p++;
            }
        }
    }
    if (year == 0 && month == 0 && day == 0 && hour == 0 && minute == 0
        && second == 0)
    {
        clif_displaymessage(fd, "Invalid time for ban command.");
        return -1;
    }

    // check player name
    if (strlen(character) < 4)
    {
        clif_displaymessage(fd, "Sorry, but a player name have at least 4 characters.");
        return -1;
    }
    else if (strlen(character) > 23)
    {
        clif_displaymessage(fd, "Sorry, but a player name have 23 characters maximum.");
        return -1;
    }
    else
    {
        chrif_char_ask_name(sd->status.account_id, character, 2, year, month, day, hour, minute, second);  // type: 2 - ban
        clif_displaymessage(fd, "Character name sends to char-server to ask it.");
    }

    return 0;
}

/*==========================================
 * charunblock command (usage: charunblock <player_name>)
 *------------------------------------------
 */
int atcommand_char_unblock(const int fd, struct map_session_data *sd,
                            const char *, const char *message)
{
    char character[100];

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charunblock <player_name>).");
        return -1;
    }

    // check player name
    if (strlen(character) < 4)
    {
        clif_displaymessage(fd, "Sorry, but a player name have at least 4 characters.");
        return -1;
    }
    else if (strlen(character) > 23)
    {
        clif_displaymessage(fd, "Sorry, but a player name have 23 characters maximum.");
        return -1;
    }
    else
    {
        // send answer to login server via char-server
        chrif_char_ask_name(sd->status.account_id, character, 3, 0, 0, 0, 0, 0, 0);    // type: 3 - unblock
        clif_displaymessage(fd, "Character name sends to char-server to ask it.");
    }

    return 0;
}

/*==========================================
 * charunban command (usage: charunban <player_name>)
 *------------------------------------------
 */
int atcommand_char_unban(const int fd, struct map_session_data *sd,
                          const char *, const char *message)
{
    char character[100];

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charunban <player_name>).");
        return -1;
    }

    // check player name
    if (strlen(character) < 4)
    {
        clif_displaymessage(fd, "Sorry, but a player name have at least 4 characters.");
        return -1;
    }
    else if (strlen(character) > 23)
    {
        clif_displaymessage(fd, "Sorry, but a player name have 23 characters maximum.");
        return -1;
    }
    else
    {
        // send answer to login server via char-server
        chrif_char_ask_name(sd->status.account_id, character, 4, 0, 0, 0, 0, 0, 0);    // type: 4 - unban
        clif_displaymessage(fd, "Character name sends to char-server to ask it.");
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_character_save(const int fd, struct map_session_data *sd,
                              const char *, const char *message)
{
    char map_name[100];
    char character[100];
    struct map_session_data *pl_sd;
    int x = 0, y = 0;
    int m;

    memset(map_name, '\0', sizeof(map_name));
    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%99s %d %d %99[^\n]", map_name, &x, &y,
                   character) < 4 || x < 0 || y < 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a valid save point and a player name (usage: @charsave <map> <x> <y> <charname>).");
        return -1;
    }

    if (strstr(map_name, ".gat") == NULL && strstr(map_name, ".afm") == NULL && strlen(map_name) < 13)   // 16 - 4 (.gat)
        strcat(map_name, ".gat");

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can change save point only to lower or same gm level
            m = map_mapname2mapid(map_name);
            if (m < 0)
            {
                clif_displaymessage(fd, "Map not found.");
                return -1;
            }
            else
            {
                if (m >= 0 && map[m].flag.nowarpto
                    && battle_config.any_warp_GM_min_level > pc_isGM(sd))
                {
                    clif_displaymessage(fd,
                                         "You are not authorised to set this map as a save map.");
                    return -1;
                }
                pc_setsavepoint(pl_sd, map_name, x, y);
                clif_displaymessage(fd, "Character's respawn point changed.");
            }
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_doom(const int fd, struct map_session_data *sd,
                    const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;

    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth && i != fd
            && pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can doom only lower or same gm level
            pc_damage(NULL, pl_sd, pl_sd->status.hp + 1);
            clif_displaymessage(pl_sd->fd, "The holy messenger has given judgement.");
        }
    }
    clif_displaymessage(fd, "Judgement was made.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_doommap(const int fd, struct map_session_data *sd,
                       const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;

    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth && i != fd && sd->bl.m == pl_sd->bl.m
            && pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can doom only lower or same gm level
            pc_damage(NULL, pl_sd, pl_sd->status.hp + 1);
            clif_displaymessage(pl_sd->fd, "The holy messenger has given judgement.");
        }
    }
    clif_displaymessage(fd, "Judgement was made.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
static
void atcommand_raise_sub(struct map_session_data *sd)
{
    if (sd && sd->state.auth && pc_isdead(sd))
    {
        sd->status.hp = sd->status.max_hp;
        sd->status.sp = sd->status.max_sp;
        pc_setstand(sd);
        clif_updatestatus(sd, SP::HP);
        clif_updatestatus(sd, SP::SP);
        clif_resurrection(&sd->bl, 1);
        clif_displaymessage(sd->fd, "Mercy has been shown.");
    }
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_raise(const int fd, struct map_session_data *,
                     const char *, const char *)
{
    int i;

    for (i = 0; i < fd_max; i++)
    {
        if (session[i])
            atcommand_raise_sub((struct map_session_data *)session[i]->session_data);
    }
    clif_displaymessage(fd, "Mercy has been granted.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_raisemap(const int fd, struct map_session_data *sd,
                        const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;

    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth && sd->bl.m == pl_sd->bl.m)
            atcommand_raise_sub(pl_sd);
    }
    clif_displaymessage(fd, "Mercy has been granted.");

    return 0;
}

/*==========================================
 * atcommand_character_baselevel @charbaselvlで対象キャラのレベルを上げる
 *------------------------------------------
*/
int atcommand_character_baselevel(const int fd, struct map_session_data *sd,
                                   const char *, const char *message)
{
    struct map_session_data *pl_sd;
    char character[100];
    int level = 0, i;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &level, character) < 2
        || level == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a level adjustement and a player name (usage: @charblvl <#> <nickname>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can change base level only lower or same gm level

            if (level > 0)
            {
                if (pl_sd->status.base_level == battle_config.maximum_level)
                {               // check for max level by Valaris
                    clif_displaymessage(fd, "Character's base level can't go any higher.");
                    return 0;
                }               // End Addition
                if (level > battle_config.maximum_level || level > (battle_config.maximum_level - pl_sd->status.base_level))    // fix positiv overflow
                    level =
                        battle_config.maximum_level -
                        pl_sd->status.base_level;
                for (i = 1; i <= level; i++)
                    pl_sd->status.status_point +=
                        (pl_sd->status.base_level + i + 14) / 4;
                pl_sd->status.base_level += level;
                clif_updatestatus(pl_sd, SP::BASELEVEL);
                clif_updatestatus(pl_sd, SP::NEXTBASEEXP);
                clif_updatestatus(pl_sd, SP::STATUSPOINT);
                pc_calcstatus(pl_sd, 0);
                pc_heal(pl_sd, pl_sd->status.max_hp, pl_sd->status.max_sp);
                clif_misceffect(&pl_sd->bl, 0);
                clif_displaymessage(fd, "Character's base level raised.");
            }
            else
            {
                if (pl_sd->status.base_level == 1)
                {
                    clif_displaymessage(fd, "Character's base level can't go any lower.");
                    return -1;
                }
                if (level < -battle_config.maximum_level || level < (1 - pl_sd->status.base_level)) // fix negativ overflow
                    level = 1 - pl_sd->status.base_level;
                if (pl_sd->status.status_point > 0)
                {
                    for (i = 0; i > level; i--)
                        pl_sd->status.status_point -=
                            (pl_sd->status.base_level + i + 14) / 4;
                    if (pl_sd->status.status_point < 0)
                        pl_sd->status.status_point = 0;
                    clif_updatestatus(pl_sd, SP::STATUSPOINT);
                }               // to add: remove status points from stats
                pl_sd->status.base_level += level;
                pl_sd->status.base_exp = 0;
                clif_updatestatus(pl_sd, SP::BASELEVEL);
                clif_updatestatus(pl_sd, SP::NEXTBASEEXP);
                clif_updatestatus(pl_sd, SP::BASEEXP);
                pc_calcstatus(pl_sd, 0);
                clif_displaymessage(fd, "Character's base level lowered.");
            }
            // Reset their stat points to prevent extra points from stacking
            atcommand_charstreset(fd, sd,"@charstreset", character);
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;                   //正常終了
}

/*==========================================
 * atcommand_character_joblevel @charjoblvlで対象キャラのJobレベルを上げる
 *------------------------------------------
 */
int atcommand_character_joblevel(const int fd, struct map_session_data *sd,
                                  const char *, const char *message)
{
    struct map_session_data *pl_sd;
    char character[100];
    int max_level = 50, level = 0;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &level, character) < 2
        || level == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a level adjustement and a player name (usage: @charjlvl <#> <nickname>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {
            // you can change job level only lower or same gm level
            max_level -= 40;

            if (level > 0)
            {
                if (pl_sd->status.job_level == max_level)
                {
                    clif_displaymessage(fd, "Character's job level can't go any higher.");
                    return -1;
                }
                if (pl_sd->status.job_level + level > max_level)
                    level = max_level - pl_sd->status.job_level;
                pl_sd->status.job_level += level;
                clif_updatestatus(pl_sd, SP::JOBLEVEL);
                clif_updatestatus(pl_sd, SP::NEXTJOBEXP);
                pl_sd->status.skill_point += level;
                clif_updatestatus(pl_sd, SP::SKILLPOINT);
                pc_calcstatus(pl_sd, 0);
                clif_misceffect(&pl_sd->bl, 1);
                clif_displaymessage(fd, "character's job level raised.");
            }
            else
            {
                if (pl_sd->status.job_level == 1)
                {
                    clif_displaymessage(fd, "Character's job level can't go any lower.");
                    return -1;
                }
                if (pl_sd->status.job_level + level < 1)
                    level = 1 - pl_sd->status.job_level;
                pl_sd->status.job_level += level;
                clif_updatestatus(pl_sd, SP::JOBLEVEL);
                clif_updatestatus(pl_sd, SP::NEXTJOBEXP);
                if (pl_sd->status.skill_point > 0)
                {
                    pl_sd->status.skill_point += level;
                    if (pl_sd->status.skill_point < 0)
                        pl_sd->status.skill_point = 0;
                    clif_updatestatus(pl_sd, SP::SKILLPOINT);
                }               // to add: remove status points from skills
                pc_calcstatus(pl_sd, 0);
                clif_displaymessage(fd, "Character's job level lowered.");
            }
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_kick(const int fd, struct map_session_data *sd,
                    const char *, const char *message)
{
    struct map_session_data *pl_sd;
    char character[100];

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @kick <charname>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))    // you can kick only lower or same gm level
            clif_GM_kick(sd, pl_sd, 1);
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_kickall(const int fd, struct map_session_data *sd,
                       const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;

    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth && pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can kick only lower or same gm level
            if (sd->status.account_id != pl_sd->status.account_id)
                clif_GM_kick(sd, pl_sd, 0);
        }
    }

    clif_displaymessage(fd, "All players have been kicked!");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_questskill(const int fd, struct map_session_data *sd,
                          const char *, const char *message)
{
    int skill_id_;

    if (!message || !*message || (skill_id_ = atoi(message)) < 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a quest skill number (usage: @questskill <#:0+>).");
        return -1;
    }

    SkillID skill_id = SkillID(skill_id_);

    if (/*skill_id >= SkillID() &&*/ skill_id < SkillID::MAX_SKILL_DB)
    {
        if (skill_get_inf2(skill_id) & 0x01)
        {
            if (pc_checkskill(sd, skill_id) == 0)
            {
                pc_skill(sd, skill_id, 1, 0);
                clif_displaymessage(fd, "You have learned the skill.");
            }
            else
            {
                clif_displaymessage(fd, "You already have this quest skill.");
                return -1;
            }
        }
        else
        {
            clif_displaymessage(fd, "This skill number doesn't exist or isn't a quest skill.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "This skill number doesn't exist.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_charquestskill(const int fd, struct map_session_data *,
                              const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;
    int skill_id_ = 0;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &skill_id_, character) < 2
        || skill_id_ < 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a quest skill number and a player name (usage: @charquestskill <#:0+> <char_name>).");
        return -1;
    }

    SkillID skill_id = SkillID(skill_id_);

    if (/*skill_id >= SkillID() &&*/ skill_id < SkillID::MAX_SKILL_DB)
    {
        if (skill_get_inf2(skill_id) & 0x01)
        {
            if ((pl_sd = map_nick2sd(character)) != NULL)
            {
                if (pc_checkskill(pl_sd, skill_id) == 0)
                {
                    pc_skill(pl_sd, skill_id, 1, 0);
                    clif_displaymessage(fd, "This player has learned the skill.");
                }
                else
                {
                    clif_displaymessage(fd, "This player already has this quest skill.");
                    return -1;
                }
            }
            else
            {
                clif_displaymessage(fd, "Character not found.");
                return -1;
            }
        }
        else
        {
            clif_displaymessage(fd, "This skill number doesn't exist or isn't a quest skill.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "This skill number doesn't exist.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_lostskill(const int fd, struct map_session_data *sd,
                         const char *, const char *message)
{
    int skill_id_;

    if (!message || !*message || (skill_id_ = atoi(message)) < 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a quest skill number (usage: @lostskill <#:0+>).");
        return -1;
    }

    SkillID skill_id = SkillID(skill_id_);

    if (/*skill_id >= SkillID() &&*/ skill_id < MAX_SKILL)
    {
        if (skill_get_inf2(skill_id) & 0x01)
        {
            if (pc_checkskill(sd, skill_id) > 0)
            {
                sd->status.skill[skill_id].lv = 0;
                sd->status.skill[skill_id].flags = SkillFlags::ZERO;
                clif_skillinfoblock(sd);
                clif_displaymessage(fd, "You have forgotten the skill.");
            }
            else
            {
                clif_displaymessage(fd, "You don't have this quest skill.");
                return -1;
            }
        }
        else
        {
            clif_displaymessage(fd, "This skill number doesn't exist or isn't a quest skill.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "This skill number doesn't exist.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_charlostskill(const int fd, struct map_session_data *,
                             const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;
    int skill_id_ = 0;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &skill_id_, character) < 2
        || skill_id_ < 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a quest skill number and a player name (usage: @charlostskill <#:0+> <char_name>).");
        return -1;
    }

    SkillID skill_id = SkillID(skill_id_);

    if (/*skill_id >= SkillID() &&*/ skill_id < MAX_SKILL)
    {
        if (skill_get_inf2(skill_id) & 0x01)
        {
            if ((pl_sd = map_nick2sd(character)) != NULL)
            {
                if (pc_checkskill(pl_sd, skill_id) > 0)
                {
                    pl_sd->status.skill[skill_id].lv = 0;
                    pl_sd->status.skill[skill_id].flags = SkillFlags::ZERO;
                    clif_skillinfoblock(pl_sd);
                    clif_displaymessage(fd, "This player has forgotten the skill.");
                }
                else
                {
                    clif_displaymessage(fd, "This player doesn't have this quest skill.");
                    return -1;
                }
            }
            else
            {
                clif_displaymessage(fd, "Character not found.");
                return -1;
            }
        }
        else
        {
            clif_displaymessage(fd, "This skill number doesn't exist or isn't a quest skill.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "This skill number doesn't exist.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_party(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    char party[100];

    memset(party, '\0', sizeof(party));

    if (!message || !*message || sscanf(message, "%99[^\n]", party) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a party name (usage: @party <party_name>).");
        return -1;
    }

    party_create(sd, party);

    return 0;
}

/*==========================================
 * @mapexitでマップサーバーを終了させる
 *------------------------------------------
 */
int atcommand_mapexit(const int, struct map_session_data *sd,
                       const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;

    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth)
        {
            if (sd->status.account_id != pl_sd->status.account_id)
                clif_GM_kick(sd, pl_sd, 0);
        }
    }
    clif_GM_kick(sd, sd, 0);

    runflag = 0;

    return 0;
}

/*==========================================
 * idsearch <part_of_name>: revrited by [Yor]
 *------------------------------------------
 */
int atcommand_idsearch(const int fd, struct map_session_data *,
                        const char *, const char *message)
{
    char item_name[100];
    int i, match;
    struct item_data *item;

    memset(item_name, '\0', sizeof(item_name));

    if (!message || !*message || sscanf(message, "%99s", item_name) < 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a part of item name (usage: @idsearch <part_of_item_name>).");
        return -1;
    }

    std::string output = STRPRINTF("The reference result of '%s' (name: id):", item_name);
    clif_displaymessage(fd, output);
    match = 0;
    for (i = 0; i < 20000; i++)
    {
        if ((item = itemdb_exists(i)) != NULL
            && strstr(item->jname, item_name) != NULL)
        {
            match++;
            output = STRPRINTF("%s: %d", item->jname, item->nameid);
            clif_displaymessage(fd, output);
        }
    }
    output = STRPRINTF("It is %d affair above.", match);
    clif_displaymessage(fd, output);

    return 0;
}

/*==========================================
 * Character Skill Reset
 *------------------------------------------
 */
int atcommand_charskreset(const int fd, struct map_session_data *sd,
                           const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charskreset <charname>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can reset skill points only lower or same gm level
            pc_resetskill(pl_sd);
            std::string output = STRPRINTF(
                    "'%s' skill points reseted!", character);
            clif_displaymessage(fd, output);
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * Character Stat Reset
 *------------------------------------------
 */
int atcommand_charstreset(const int fd, struct map_session_data *sd,
                           const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charstreset <charname>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can reset stats points only lower or same gm level
            pc_resetstate(pl_sd);
            std::string output = STRPRINTF(
                    "'%s' stats points reseted!",
                    character);
            clif_displaymessage(fd, output);
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * Character Reset
 *------------------------------------------
 */
int atcommand_charreset(const int fd, struct map_session_data *sd,
                         const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charreset <charname>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can reset a character only for lower or same GM level
            pc_resetstate(pl_sd);
            pc_resetskill(pl_sd);
            pc_setglobalreg(pl_sd, "MAGIC_FLAGS", 0);  // [Fate] Reset magic quest variables
            pc_setglobalreg(pl_sd, "MAGIC_EXP", 0);    // [Fate] Reset magic experience
            std::string output = STRPRINTF(
                    "'%s' skill and stats points reseted!", character);
            clif_displaymessage(fd, output);
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * Character Wipe
 *------------------------------------------
 */
int atcommand_char_wipe(const int fd, struct map_session_data *sd,
                         const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charwipe <charname>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can reset a character only for lower or same GM level
            int i;

            // Reset base level
            pl_sd->status.base_level = 1;
            pl_sd->status.base_exp = 0;
            clif_updatestatus(pl_sd, SP::BASELEVEL);
            clif_updatestatus(pl_sd, SP::NEXTBASEEXP);
            clif_updatestatus(pl_sd, SP::BASEEXP);

            // Reset job level
            pl_sd->status.job_level = 1;
            pl_sd->status.job_exp = 0;
            clif_updatestatus(pl_sd, SP::JOBLEVEL);
            clif_updatestatus(pl_sd, SP::NEXTJOBEXP);
            clif_updatestatus(pl_sd, SP::JOBEXP);

            // Zeny to 50
            pl_sd->status.zeny = 50;
            clif_updatestatus(pl_sd, SP::ZENY);

            // Clear inventory
            for (i = 0; i < MAX_INVENTORY; i++)
            {
                if (sd->status.inventory[i].amount)
                {
                    if (bool(sd->status.inventory[i].equip))
                        pc_unequipitem(pl_sd, i, CalcStatus::NOW);
                    pc_delitem(pl_sd, i, sd->status.inventory[i].amount, 0);
                }
            }

            // Give knife and shirt
            struct item item;
            item.nameid = 1201; // knife
            item.identify = 1;
            item.broken = 0;
            pc_additem(pl_sd, &item, 1);
            item.nameid = 1202; // shirt
            pc_additem(pl_sd, &item, 1);

            // Reset stats and skills
            pc_calcstatus(pl_sd, 0);
            pc_resetstate(pl_sd);
            pc_resetskill(pl_sd);
            pc_setglobalreg(pl_sd, "MAGIC_FLAGS", 0);  // [Fate] Reset magic quest variables
            pc_setglobalreg(pl_sd, "MAGIC_EXP", 0);    // [Fate] Reset magic experience

            std::string output = STRPRINTF("%s:  wiped.", character);
            clif_displaymessage(fd, output);
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * Character Model by chbrules
 *------------------------------------------
 */
int atcommand_charmodel(const int fd, struct map_session_data *,
                         const char *, const char *message)
{
    int hair_style = 0, hair_color = 0, cloth_color = 0;
    struct map_session_data *pl_sd;
    char character[100];

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %d %d %99[^\n]", &hair_style, &hair_color,
                   &cloth_color, character) < 4 || hair_style < 0
        || hair_color < 0 || cloth_color < 0)
    {
        std::string output = STRPRINTF(
                "Please, enter a valid model and a player name (usage: @charmodel <hair ID: %d-%d> <hair color: %d-%d> <clothes color: %d-%d> <name>).",
                MIN_HAIR_STYLE, MAX_HAIR_STYLE,
                MIN_HAIR_COLOR, MAX_HAIR_COLOR,
                MIN_CLOTH_COLOR, MAX_CLOTH_COLOR);
        clif_displaymessage(fd, output);
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (hair_style >= MIN_HAIR_STYLE && hair_style <= MAX_HAIR_STYLE &&
            hair_color >= MIN_HAIR_COLOR && hair_color <= MAX_HAIR_COLOR &&
            cloth_color >= MIN_CLOTH_COLOR && cloth_color <= MAX_CLOTH_COLOR)
        {
            {
                pc_changelook(pl_sd, LOOK::HAIR, hair_style);
                pc_changelook(pl_sd, LOOK::HAIR_COLOR, hair_color);
                pc_changelook(pl_sd, LOOK::CLOTHES_COLOR, cloth_color);
                clif_displaymessage(fd, "Appearence changed.");
            }
        }
        else
        {
            clif_displaymessage(fd, "An invalid number was specified.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * Character Skill Point (Rewritten by [Yor])
 *------------------------------------------
 */
int atcommand_charskpoint(const int fd, struct map_session_data *,
                           const char *, const char *message)
{
    struct map_session_data *pl_sd;
    char character[100];
    int new_skill_point;
    int point = 0;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &point, character) < 2
        || point == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a number and a player name (usage: @charskpoint <amount> <name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        new_skill_point = (int) pl_sd->status.skill_point + point;
        if (point > 0 && (point > 0x7FFF || new_skill_point > 0x7FFF))  // fix positiv overflow
            new_skill_point = 0x7FFF;
        else if (point < 0 && (point < -0x7FFF || new_skill_point < 0)) // fix negativ overflow
            new_skill_point = 0;
        if (new_skill_point != (int) pl_sd->status.skill_point)
        {
            pl_sd->status.skill_point = new_skill_point;
            clif_updatestatus(pl_sd, SP::SKILLPOINT);
            clif_displaymessage(fd, "Character's number of skill points changed!");
        }
        else
        {
            if (point < 0)
                clif_displaymessage(fd, "Impossible to decrease the number/value.");
            else
                clif_displaymessage(fd, "Impossible to increase the number/value.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * Character Status Point (rewritten by [Yor])
 *------------------------------------------
 */
int atcommand_charstpoint(const int fd, struct map_session_data *,
                           const char *, const char *message)
{
    struct map_session_data *pl_sd;
    char character[100];
    int new_status_point;
    int point = 0;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &point, character) < 2
        || point == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a number and a player name (usage: @charstpoint <amount> <name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        new_status_point = (int) pl_sd->status.status_point + point;
        if (point > 0 && (point > 0x7FFF || new_status_point > 0x7FFF)) // fix positiv overflow
            new_status_point = 0x7FFF;
        else if (point < 0 && (point < -0x7FFF || new_status_point < 0))    // fix negativ overflow
            new_status_point = 0;
        if (new_status_point != (int) pl_sd->status.status_point)
        {
            pl_sd->status.status_point = new_status_point;
            clif_updatestatus(pl_sd, SP::STATUSPOINT);
            clif_displaymessage(fd, "Character's number of status points changed!");
        }
        else
        {
            if (point < 0)
                clif_displaymessage(fd, "Impossible to decrease the number/value.");
            else
                clif_displaymessage(fd, "Impossible to increase the number/value.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * Character Zeny Point (Rewritten by [Yor])
 *------------------------------------------
 */
int atcommand_charzeny(const int fd, struct map_session_data *,
                        const char *, const char *message)
{
    struct map_session_data *pl_sd;
    char character[100];
    int zeny = 0, new_zeny;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &zeny, character) < 2 || zeny == 0)
    {
        clif_displaymessage(fd,
                             "Please, enter a number and a player name (usage: @charzeny <zeny> <name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        new_zeny = pl_sd->status.zeny + zeny;
        if (zeny > 0 && (zeny > MAX_ZENY || new_zeny > MAX_ZENY))   // fix positiv overflow
            new_zeny = MAX_ZENY;
        else if (zeny < 0 && (zeny < -MAX_ZENY || new_zeny < 0))    // fix negativ overflow
            new_zeny = 0;
        if (new_zeny != pl_sd->status.zeny)
        {
            pl_sd->status.zeny = new_zeny;
            clif_updatestatus(pl_sd, SP::ZENY);
            clif_displaymessage(fd, "Character's number of zenys changed!");
        }
        else
        {
            if (zeny < 0)
                clif_displaymessage(fd, "Impossible to decrease the number/value.");
            else
                clif_displaymessage(fd, "Impossible to increase the number/value.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * Recall All Characters Online To Your Location
 *------------------------------------------
 */
int atcommand_recallall(const int fd, struct map_session_data *sd,
                         const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;
    int count;

    if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarpto
        && battle_config.any_warp_GM_min_level > pc_isGM(sd))
    {
        clif_displaymessage(fd,
                             "You are not authorised to warp somenone to your actual map.");
        return -1;
    }

    count = 0;
    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth
            && sd->status.account_id != pl_sd->status.account_id
            && pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can recall only lower or same level
            if (pl_sd->bl.m >= 0 && map[pl_sd->bl.m].flag.nowarp
                && battle_config.any_warp_GM_min_level > pc_isGM(sd))
                count++;
            else
                pc_setpos(pl_sd, sd->mapname, sd->bl.x, sd->bl.y, BeingRemoveWhy::QUIT);
        }
    }

    clif_displaymessage(fd, "All characters recalled!");
    if (count)
    {
        std::string output = STRPRINTF(
                "Because you are not authorised to warp from some maps, %d player(s) have not been recalled.",
                count);
        clif_displaymessage(fd, output);
    }

    return 0;
}

/*==========================================
 * Recall online characters of a party to your location
 *------------------------------------------
 */
int atcommand_partyrecall(const int fd, struct map_session_data *sd,
                           const char *, const char *message)
{
    int i;
    struct map_session_data *pl_sd;
    char party_name[100];
    struct party *p;
    int count;

    memset(party_name, '\0', sizeof(party_name));

    if (!message || !*message || sscanf(message, "%99[^\n]", party_name) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a party name/id (usage: @partyrecall <party_name/id>).");
        return -1;
    }

    if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarpto
        && battle_config.any_warp_GM_min_level > pc_isGM(sd))
    {
        clif_displaymessage(fd,
                             "You are not authorised to warp somenone to your actual map.");
        return -1;
    }

    if ((p = party_searchname(party_name)) != NULL ||  // name first to avoid error when name begin with a number
        (p = party_search(atoi(message))) != NULL)
    {
        count = 0;
        for (i = 0; i < fd_max; i++)
        {
            if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
                && pl_sd->state.auth
                && sd->status.account_id != pl_sd->status.account_id
                && pl_sd->status.party_id == p->party_id)
            {
                if (pl_sd->bl.m >= 0 && map[pl_sd->bl.m].flag.nowarp
                    && battle_config.any_warp_GM_min_level > pc_isGM(sd))
                    count++;
                else
                    pc_setpos(pl_sd, sd->mapname, sd->bl.x, sd->bl.y, BeingRemoveWhy::QUIT);
            }
        }
        std::string output = STRPRINTF("All online characters of the %s party are near you.", p->name);
        clif_displaymessage(fd, output);
        if (count)
        {
            output = STRPRINTF(
                    "Because you are not authorised to warp from some maps, %d player(s) have not been recalled.",
                    count);
            clif_displaymessage(fd, output);
        }
    }
    else
    {
        clif_displaymessage(fd, "Incorrect name or ID, or no one from the party is online.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_reloaditemdb(const int fd, struct map_session_data *,
                            const char *, const char *)
{
    itemdb_reload();
    clif_displaymessage(fd, "Item database reloaded.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_reloadmobdb(const int fd, struct map_session_data *,
                           const char *, const char *)
{
    mob_reload();
    clif_displaymessage(fd, "Monster database reloaded.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_reloadskilldb(const int fd, struct map_session_data *,
                             const char *, const char *)
{
    skill_reload();
    clif_displaymessage(fd, "Skill database reloaded.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_reloadscript(const int fd, struct map_session_data *,
                            const char *, const char *)
{
    do_init_npc();
    do_init_script();

    npc_event_do_oninit();

    clif_displaymessage(fd, "Scripts reloaded.");

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_reloadgmdb(const int fd, struct map_session_data *,
        const char *, const char *)
{
    chrif_reloadGMdb();

    clif_displaymessage(fd, "Login-server asked to reload GM accounts and their level.");

    return 0;
}

/*==========================================
 * @mapinfo <map name> [0-3] by MC_Cameri
 * => Shows information about the map [map name]
 * 0 = no additional information
 * 1 = Show users in that map and their location
 * 2 = Shows NPCs in that map
 * 3 = Shows the shops/chats in that map (not implemented)
 *------------------------------------------
 */
int atcommand_mapinfo(const int fd, struct map_session_data *sd,
                       const char *, const char *message)
{
    struct map_session_data *pl_sd;
    struct npc_data *nd = NULL;
    struct chat_data *cd = NULL;
    char map_name[100];
    const char *direction = NULL;
    int m_id, i, chat_num, list = 0;

    memset(map_name, '\0', sizeof(map_name));

    sscanf(message, "%d %99[^\n]", &list, map_name);

    if (list < 0 || list > 3)
    {
        clif_displaymessage(fd,
                             "Please, enter at least a valid list number (usage: @mapinfo <0-3> [map]).");
        return -1;
    }

    if (map_name[0] == '\0')
        strcpy(map_name, sd->mapname);
    if (strstr(map_name, ".gat") == NULL && strstr(map_name, ".afm") == NULL && strlen(map_name) < 13)   // 16 - 4 (.gat)
        strcat(map_name, ".gat");

    if ((m_id = map_mapname2mapid(map_name)) < 0)
    {
        clif_displaymessage(fd, "Map not found.");
        return -1;
    }

    clif_displaymessage(fd, "------ Map Info ------");
    std::string output = STRPRINTF("Map Name: %s", map_name);
    clif_displaymessage(fd, output);
    output = STRPRINTF("Players In Map: %d", map[m_id].users);
    clif_displaymessage(fd, output);
    output = STRPRINTF("NPCs In Map: %d", map[m_id].npc_num);
    clif_displaymessage(fd, output);
    chat_num = 0;
    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth
            && (cd = (struct chat_data *) map_id2bl(pl_sd->chatID)))
        {
            chat_num++;
        }
    }
    output = STRPRINTF("Chats In Map: %d", chat_num);
    clif_displaymessage(fd, output);
    clif_displaymessage(fd, "------ Map Flags ------");
    output = STRPRINTF("Player vs Player: %s | No Party: %s",
             (map[m_id].flag.pvp) ? "True" : "False",
             (map[m_id].flag.pvp_noparty) ? "True" : "False");
    clif_displaymessage(fd, output);
    output = STRPRINTF("No Dead Branch: %s",
             (map[m_id].flag.nobranch) ? "True" : "False");
    clif_displaymessage(fd, output);
    output = STRPRINTF("No Memo: %s",
             (map[m_id].flag.nomemo) ? "True" : "False");
    clif_displaymessage(fd, output);
    output = STRPRINTF("No Penalty: %s",
             (map[m_id].flag.nopenalty) ? "True" : "False");
    clif_displaymessage(fd, output);
    output = STRPRINTF("No Return: %s",
             (map[m_id].flag.noreturn) ? "True" : "False");
    clif_displaymessage(fd, output);
    output = STRPRINTF("No Save: %s",
             (map[m_id].flag.nosave) ? "True" : "False");
    clif_displaymessage(fd, output);
    output = STRPRINTF("No Teleport: %s",
             (map[m_id].flag.noteleport) ? "True" : "False");
    clif_displaymessage(fd, output);
    output = STRPRINTF("No Monster Teleport: %s",
             (map[m_id].flag.monster_noteleport) ? "True" : "False");
    clif_displaymessage(fd, output);
    output = STRPRINTF("No Zeny Penalty: %s",
             (map[m_id].flag.nozenypenalty) ? "True" : "False");
    clif_displaymessage(fd, output);

    switch (list)
    {
        case 0:
            // Do nothing. It's list 0, no additional display.
            break;
        case 1:
            clif_displaymessage(fd, "----- Players in Map -----");
            for (i = 0; i < fd_max; i++)
            {
                if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
                    && pl_sd->state.auth
                    && strcmp(pl_sd->mapname, map_name) == 0)
                {
                    output = STRPRINTF(
                             "Player '%s' (session #%d) | Location: %d,%d",
                             pl_sd->status.name, i, pl_sd->bl.x, pl_sd->bl.y);
                    clif_displaymessage(fd, output);
                }
            }
            break;
        case 2:
            clif_displaymessage(fd, "----- NPCs in Map -----");
            for (i = 0; i < map[m_id].npc_num;)
            {
                nd = map[m_id].npc[i];
                switch (nd->dir)
                {
                    case DIR::S:
                        direction = "North";
                        break;
                    case DIR::SW:
                        direction = "North West";
                        break;
                    case DIR::W:
                        direction = "West";
                        break;
                    case DIR::NW:
                        direction = "South West";
                        break;
                    case DIR::N:
                        direction = "South";
                        break;
                    case DIR::NE:
                        direction = "South East";
                        break;
                    case DIR::E:
                        direction = "East";
                        break;
                    case DIR::SE:
                        direction = "North East";
                        break;
#if 0
                    case 9:
                        direction = "North";
                        break;
#endif
                    default:
                        direction = "Unknown";
                        break;
                }
                output = STRPRINTF(
                         "NPC %d: %s | Direction: %s | Sprite: %d | Location: %d %d",
                         ++i, nd->name, direction, nd->npc_class, nd->bl.x,
                         nd->bl.y);
                clif_displaymessage(fd, output);
            }
            break;
        case 3:
            clif_displaymessage(fd, "----- Chats in Map -----");
            for (i = 0; i < fd_max; i++)
            {
                if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
                    && pl_sd->state.auth
                    && (cd = (struct chat_data *) map_id2bl(pl_sd->chatID))
                    && strcmp(pl_sd->mapname, map_name) == 0
                    && cd->usersd[0] == pl_sd)
                {
                    output = STRPRINTF(
                             "Chat %d: %s | Player: %s | Location: %d %d", i,
                             cd->title, pl_sd->status.name, cd->bl.x,
                             cd->bl.y);
                    clif_displaymessage(fd, output);
                    output = STRPRINTF(
                             "   Users: %d/%d | Password: %s | Public: %s",
                             cd->users, cd->limit, cd->pass,
                             (cd->pub) ? "Yes" : "No");
                    clif_displaymessage(fd, output);
                }
            }
            break;
        default:               // normally impossible to arrive here
            clif_displaymessage(fd,
                                 "Please, enter at least a valid list number (usage: @mapinfo <0-3> [map]).");
            return -1;
    }

    return 0;
}

/*==========================================
 *Spy Commands by Syrus22
 *------------------------------------------
 */
int atcommand_partyspy(const int fd, struct map_session_data *sd,
                        const char *, const char *message)
{
    char party_name[100];
    struct party *p;

    memset(party_name, '\0', sizeof(party_name));

    if (!message || !*message || sscanf(message, "%99[^\n]", party_name) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a party name/id (usage: @partyspy <party_name/id>).");
        return -1;
    }

    if ((p = party_searchname(party_name)) != NULL ||  // name first to avoid error when name begin with a number
        (p = party_search(atoi(message))) != NULL)
    {
        if (sd->partyspy == p->party_id)
        {
            sd->partyspy = 0;
            std::string output = STRPRINTF("No longer spying on the %s party.", p->name);
            clif_displaymessage(fd, output);
        }
        else
        {
            sd->partyspy = p->party_id;
            std::string output = STRPRINTF("Spying on the %s party.", p->name);
            clif_displaymessage(fd, output);
        }
    }
    else
    {
        clif_displaymessage(fd, "Incorrect name or ID, or no one from the party is online.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_enablenpc(const int fd, struct map_session_data *,
                         const char *, const char *message)
{
    char NPCname[100];

    memset(NPCname, '\0', sizeof(NPCname));

    if (!message || !*message || sscanf(message, "%99[^\n]", NPCname) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a NPC name (usage: @npcon <NPC_name>).");
        return -1;
    }

    if (npc_name2id(NPCname) != NULL)
    {
        npc_enable(NPCname, 1);
        clif_displaymessage(fd, "Npc Enabled.");
    }
    else
    {
        clif_displaymessage(fd, "This NPC doesn't exist.");
        return -1;
    }

    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_disablenpc(const int fd, struct map_session_data *,
                          const char *, const char *message)
{
    char NPCname[100];

    memset(NPCname, '\0', sizeof(NPCname));

    if (!message || !*message || sscanf(message, "%99[^\n]", NPCname) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a NPC name (usage: @npcoff <NPC_name>).");
        return -1;
    }

    if (npc_name2id(NPCname) != NULL)
    {
        npc_enable(NPCname, 0);
        clif_displaymessage(fd, "Npc Disabled.");
    }
    else
    {
        clif_displaymessage(fd, "This NPC doesn't exist.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @time/@date/@server_date/@serverdate/@server_time/@servertime: Display the date/time of the server (by [Yor]
 * Calculation management of GM modification (@day/@night GM commands) is done
 *------------------------------------------
 */
int atcommand_servertime(const int fd, struct map_session_data *,
                          const char *, const char *)
{
    timestamp_seconds_buffer tsbuf;
    stamp_time(tsbuf);
    std::string temp = STRPRINTF("Server time: %s", tsbuf);
    clif_displaymessage(fd, temp);

    {
        if (0 == 0)
            clif_displaymessage(fd, "Game time: The game is in permanent daylight.");
    }

    return 0;
}

/*==========================================
 * @chardelitem <item_name_or_ID> <quantity> <player> (by [Yor]
 * removes <quantity> item from a character
 * item can be equiped or not.
 * Inspired from a old command created by RoVeRT
 *------------------------------------------
 */
int atcommand_chardelitem(const int fd, struct map_session_data *sd,
                           const char *, const char *message)
{
    struct map_session_data *pl_sd;
    char character[100];
    char item_name[100];
    int i, number = 0, item_id, item_position, count;
    struct item_data *item_data;

    memset(character, '\0', sizeof(character));
    memset(item_name, '\0', sizeof(item_name));

    if (!message || !*message
        || sscanf(message, "%s %d %99[^\n]", item_name, &number,
                   character) < 3 || number < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter an item name/id, a quantity and a player name (usage: @chardelitem <item_name_or_ID> <quantity> <player>).");
        return -1;
    }

    item_id = 0;
    if ((item_data = itemdb_searchname(item_name)) != NULL ||
        (item_data = itemdb_exists(atoi(item_name))) != NULL)
        item_id = item_data->nameid;

    if (item_id > 500)
    {
        if ((pl_sd = map_nick2sd(character)) != NULL)
        {
            if (pc_isGM(sd) >= pc_isGM(pl_sd))
            {                   // you can kill only lower or same level
                item_position = pc_search_inventory(pl_sd, item_id);
                if (item_position >= 0)
                {
                    count = 0;
                    for (i = 0; i < number && item_position >= 0; i++)
                    {
                        pc_delitem(pl_sd, item_position, 1, 0);
                        count++;
                        item_position = pc_search_inventory(pl_sd, item_id);   // for next loop
                    }
                    std::string output = STRPRINTF(
                            "%d item(s) removed by a GM.",
                            count);
                    clif_displaymessage(pl_sd->fd, output);

                    if (number == count)
                        output = STRPRINTF("%d item(s) removed from the player.", count);
                    else
                        output = STRPRINTF("%d item(s) removed. Player had only %d on %d items.", count, count, number);
                    clif_displaymessage(fd, output);
                }
                else
                {
                    clif_displaymessage(fd, "Character does not have the item.");
                    return -1;
                }
            }
            else
            {
                clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
                return -1;
            }
        }
        else
        {
            clif_displaymessage(fd, "Character not found.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Invalid item ID or name.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @broadcast by [Valaris]
 *------------------------------------------
 */
int atcommand_broadcast(const int fd, struct map_session_data *sd,
                         const char *, const char *message)
{
    if (!message || !*message)
    {
        clif_displaymessage(fd,
                             "Please, enter a message (usage: @broadcast <message>).");
        return -1;
    }

    std::string output = STRPRINTF("%s : %s", sd->status.name, message);
    intif_GMmessage(output, 0);

    return 0;
}

/*==========================================
 * @localbroadcast by [Valaris]
 *------------------------------------------
 */
int atcommand_localbroadcast(const int fd, struct map_session_data *sd,
                              const char *, const char *message)
{
    if (!message || !*message)
    {
        clif_displaymessage(fd,
                             "Please, enter a message (usage: @localbroadcast <message>).");
        return -1;
    }

    std::string output = STRPRINTF("%s : %s", sd->status.name, message);

    clif_GMmessage(&sd->bl, output, 1);   // 1: ALL_SAMEMAP

    return 0;
}

/*==========================================
 * @ignorelist by [Yor]
 *------------------------------------------
 */
int atcommand_ignorelist(const int fd, struct map_session_data *sd,
                          const char *, const char *)
{
    int count;
    int i;

    count = 0;
    for (i = 0; i < (int)(sizeof(sd->ignore) / sizeof(sd->ignore[0])); i++)
        if (sd->ignore[i].name[0])
            count++;

    if (sd->ignoreAll == 0)
        if (count == 0)
            clif_displaymessage(fd, "You accept any wisp (no wisper is refused).");
        else
        {
            std::string output = STRPRINTF(
                    "You accept any wisp, except thoses from %d player (s):",
                    count);
            clif_displaymessage(fd, output);
        }
    else if (count == 0)
        clif_displaymessage(fd, "You refuse all wisps (no specifical wisper is refused).");
    else
    {
        std::string output = STRPRINTF(
                "You refuse all wisps, AND refuse wisps from %d player (s):",
                count);
        clif_displaymessage(fd, output);
    }

    if (count > 0)
        for (i = 0; i < (int)(sizeof(sd->ignore) / sizeof(sd->ignore[0]));
             i++)
            if (sd->ignore[i].name[0])
                clif_displaymessage(fd, sd->ignore[i].name);

    return 0;
}

/*==========================================
 * @charignorelist <player_name> by [Yor]
 *------------------------------------------
 */
int atcommand_charignorelist(const int fd, struct map_session_data *,
                              const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;
    int count;
    int i;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                "Please, enter a player name (usage: @charignorelist <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        count = 0;
        for (i = 0;
             i < (int)(sizeof(pl_sd->ignore) / sizeof(pl_sd->ignore[0]));
             i++)
            if (pl_sd->ignore[i].name[0])
                count++;

        if (pl_sd->ignoreAll == 0)
            if (count == 0)
            {
                std::string output = STRPRINTF(
                        "'%s' accept any wisp (no wisper is refused).",
                        pl_sd->status.name);
                clif_displaymessage(fd, output);
            }
            else
            {
                std::string output = STRPRINTF(
                        "'%s' accept any wisp, except thoses from %d player(s):",
                        pl_sd->status.name, count);
                clif_displaymessage(fd, output);
            }
        else if (count == 0)
        {
            std::string output = STRPRINTF(
                    "'%s' refuse all wisps (no specifical wisper is refused).",
                    pl_sd->status.name);
            clif_displaymessage(fd, output);
        }
        else
        {
            std::string output = STRPRINTF(
                    "'%s' refuse all wisps, AND refuse wisps from %d player(s):",
                    pl_sd->status.name, count);
            clif_displaymessage(fd, output);
        }

        if (count > 0)
            for (i = 0;
                 i <
                 (int)(sizeof(pl_sd->ignore) / sizeof(pl_sd->ignore[0]));
                 i++)
                if (pl_sd->ignore[i].name[0])
                    clif_displaymessage(fd, pl_sd->ignore[i].name);

    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @inall <player_name> by [Yor]
 *------------------------------------------
 */
int atcommand_inall(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @inall <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can change wisp option only to lower or same level
            if (pl_sd->ignoreAll == 0)
            {
                std::string output = STRPRINTF(
                        "'%s' already accepts all wispers.",
                        pl_sd->status.name);
                clif_displaymessage(fd, output);
                return -1;
            }
            else
            {
                pl_sd->ignoreAll = 0;
                std::string output = STRPRINTF(
                        "'%s' now accepts all wispers.",
                        pl_sd->status.name);
                clif_displaymessage(fd, output);
                // message to player
                clif_displaymessage(pl_sd->fd, "A GM has authorised all wispers for you.");
                WFIFOW(pl_sd->fd, 0) = 0x0d2;  // R 00d2 <type>.B <fail>.B: type: 0: deny, 1: allow, fail: 0: success, 1: fail
                WFIFOB(pl_sd->fd, 2) = 1;
                WFIFOB(pl_sd->fd, 3) = 0;  // success
                WFIFOSET(pl_sd->fd, 4);    // packet_len_table[0x0d2]
            }
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @exall <player_name> by [Yor]
 *------------------------------------------
 */
int atcommand_exall(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @exall <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can change wisp option only to lower or same level
            if (pl_sd->ignoreAll == 1)
            {
                std::string output = STRPRINTF(
                        "'%s' already blocks all wispers.",
                        pl_sd->status.name);
                clif_displaymessage(fd, output);
                return -1;
            }
            else
            {
                pl_sd->ignoreAll = 1;
                std::string output = STRPRINTF(
                        "'%s' blocks now all wispers.",
                        pl_sd->status.name);
                clif_displaymessage(fd, output);
                // message to player
                clif_displaymessage(pl_sd->fd, "A GM has blocked all wispers for you.");
                WFIFOW(pl_sd->fd, 0) = 0x0d2;  // R 00d2 <type>.B <fail>.B: type: 0: deny, 1: allow, fail: 0: success, 1: fail
                WFIFOB(pl_sd->fd, 2) = 0;
                WFIFOB(pl_sd->fd, 3) = 0;  // success
                WFIFOSET(pl_sd->fd, 4);    // packet_len_table[0x0d2]
            }
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @email <actual@email> <new@email> by [Yor]
 *------------------------------------------
 */
int atcommand_email(const int fd, struct map_session_data *sd,
                     const char *, const char *message)
{
    char actual_email[100];
    char new_email[100];

    memset(actual_email, '\0', sizeof(actual_email));
    memset(new_email, '\0', sizeof(new_email));

    if (!message || !*message
        || sscanf(message, "%99s %99s", actual_email, new_email) < 2)
    {
        clif_displaymessage(fd,
                             "Please enter 2 emails (usage: @email <actual@email> <new@email>).");
        return -1;
    }

    if (e_mail_check(actual_email) == 0)
    {
        clif_displaymessage(fd, "Invalid actual email. If you have default e-mail, type a@a.com.");   // Invalid actual email. If you have default e-mail, give a@a.com.
        return -1;
    }
    else if (e_mail_check(new_email) == 0)
    {
        clif_displaymessage(fd, "Invalid new email. Please enter a real e-mail.");
        return -1;
    }
    else if (strcasecmp(new_email, "a@a.com") == 0)
    {
        clif_displaymessage(fd, "New email must be a real e-mail.");
        return -1;
    }
    else if (strcasecmp(actual_email, new_email) == 0)
    {
        clif_displaymessage(fd, "New email must be different of the actual e-mail.");
        return -1;
    }
    else
    {
        chrif_changeemail(sd->status.account_id, actual_email, new_email);
        clif_displaymessage(fd, "Information sended to login-server via char-server.");
    }

    return 0;
}

/*==========================================
 *@effect
 *------------------------------------------
 */
int atcommand_effect(const int fd, struct map_session_data *sd,
                      const char *, const char *message)
{
    struct map_session_data *pl_sd;
    int type = 0, flag = 0, i;

    if (!message || !*message || sscanf(message, "%d %d", &type, &flag) < 2)
    {
        clif_displaymessage(fd,
                             "Please, enter at least a option (usage: @effect <type+>).");
        return -1;
    }
    if (flag <= 0)
    {
        clif_specialeffect(&sd->bl, type, flag);
        clif_displaymessage(fd, "Your Effect Has Changed.");   // Your effect has changed.
    }
    else
    {
        for (i = 0; i < fd_max; i++)
        {
            if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
                && pl_sd->state.auth)
            {
                clif_specialeffect(&pl_sd->bl, type, flag);
                clif_displaymessage(pl_sd->fd, "Your Effect Has Changed.");    // Your effect has changed.
            }
        }
    }

    return 0;
}

/*==========================================
 * @charitemlist <character>: Displays the list of a player's items.
 *------------------------------------------
 */
int atcommand_character_item_list(const int fd, struct map_session_data *sd,
                               const char *, const char *message)
{
    struct map_session_data *pl_sd;
    struct item_data *item_data, *item_temp;
    int i, j, count, counter, counter2;
    char character[100], equipstr[100];

    memset(character, '\0', sizeof(character));
    memset(equipstr, '\0', sizeof(equipstr));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charitemlist <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can look items only lower or same level
            counter = 0;
            count = 0;
            for (i = 0; i < MAX_INVENTORY; i++)
            {
                if (pl_sd->status.inventory[i].nameid > 0
                    && (item_data =
                        itemdb_search(pl_sd->status.inventory[i].nameid)) !=
                    NULL)
                {
                    counter = counter + pl_sd->status.inventory[i].amount;
                    count++;
                    if (count == 1)
                    {
                        std::string output = STRPRINTF(
                                "------ Items list of '%s' ------",
                                pl_sd->status.name);
                        clif_displaymessage(fd, output);
                    }
                    EPOS equip;
                    if (bool(equip = pl_sd->status.inventory[i].equip))
                    {
                        strcpy(equipstr, "| equiped: ");
                        if (bool(equip & EPOS::GLOVES))
                            strcat(equipstr, "robe/gargment, ");
                        if (bool(equip & EPOS::CAPE))
                            strcat(equipstr, "left accessory, ");
                        if (bool(equip & EPOS::MISC1))
                            strcat(equipstr, "body/armor, ");
                        if ((equip & (EPOS::WEAPON | EPOS::SHIELD)) == EPOS::WEAPON)
                            strcat(equipstr, "right hand, ");
                        if ((equip & (EPOS::WEAPON | EPOS::SHIELD)) == EPOS::SHIELD)
                            strcat(equipstr, "left hand, ");
                        if ((equip & (EPOS::WEAPON | EPOS::SHIELD)) == (EPOS::WEAPON | EPOS::SHIELD))
                            strcat(equipstr, "both hands, ");
                        if (bool(equip & EPOS::SHOES))
                            strcat(equipstr, "feet, ");
                        if (bool(equip & EPOS::MISC2))
                            strcat(equipstr, "right accessory, ");
                        if ((equip & (EPOS::TORSO | EPOS::HAT | EPOS::LEGS)) == EPOS::LEGS)
                            strcat(equipstr, "lower head, ");
                        if ((equip & (EPOS::TORSO | EPOS::HAT | EPOS::LEGS)) == EPOS::HAT)
                            strcat(equipstr, "top head, ");
                        if ((equip & (EPOS::TORSO | EPOS::HAT | EPOS::LEGS)) == (EPOS::HAT | EPOS::LEGS))
                            strcat(equipstr, "lower/top head, ");
                        if ((equip & (EPOS::TORSO | EPOS::HAT | EPOS::LEGS)) == EPOS::TORSO)
                            strcat(equipstr, "mid head, ");
                        if ((equip & (EPOS::TORSO | EPOS::HAT | EPOS::LEGS)) == (EPOS::TORSO | EPOS::LEGS))
                            strcat(equipstr, "lower/mid head, ");
                        if ((equip & (EPOS::TORSO | EPOS::HAT | EPOS::LEGS)) == (EPOS::TORSO | EPOS::HAT | EPOS::LEGS))
                            strcat(equipstr, "lower/mid/top head, ");
                        // remove final ', '
                        equipstr[strlen(equipstr) - 2] = '\0';
                    }
                    else
                        memset(equipstr, '\0', sizeof(equipstr));

                    std::string output;
                    if (sd->status.inventory[i].refine)
                        output = STRPRINTF("%d %s %+d (%s %+d, id: %d) %s",
                                 pl_sd->status.inventory[i].amount,
                                 item_data->name,
                                 pl_sd->status.inventory[i].refine,
                                 item_data->jname,
                                 pl_sd->status.inventory[i].refine,
                                 pl_sd->status.inventory[i].nameid, equipstr);
                    else
                        output = STRPRINTF("%d %s (%s, id: %d) %s",
                                 pl_sd->status.inventory[i].amount,
                                 item_data->name, item_data->jname,
                                 pl_sd->status.inventory[i].nameid, equipstr);
                    clif_displaymessage(fd, output);

                    output.clear();
                    counter2 = 0;
                    for (j = 0; j < item_data->slot; j++)
                    {
                        if (pl_sd->status.inventory[i].card[j])
                        {
                            if ((item_temp =
                                 itemdb_search(pl_sd->status.
                                                inventory[i].card[j])) !=
                                NULL)
                            {
                                if (output.empty())
                                    output = STRPRINTF(
                                            " -> (card(s): #%d %s (%s), ",
                                            ++counter2,
                                            item_temp->name,
                                            item_temp->jname);
                                else
                                    output += STRPRINTF(
                                            "#%d %s (%s), ",
                                            ++counter2,
                                            item_temp->name,
                                            item_temp->jname);
                            }
                        }
                    }
                    if (!output.empty())
                    {
                        // replace trailing ", "
#ifdef WORKAROUND_GCC46_LIBRARY
                        output.resize(output.size() - 1);
#else
                        output.pop_back();
#endif
                        output.back() = ')';
                        clif_displaymessage(fd, output);
                    }
                }
            }
            if (count == 0)
                clif_displaymessage(fd, "No item found on this player.");
            else
            {
                std::string output = STRPRINTF(
                        "%d item(s) found in %d kind(s) of items.",
                        counter, count);
                clif_displaymessage(fd, output);
            }
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @charstoragelist <character>: Displays the items list of a player's storage.
 *------------------------------------------
 */
int atcommand_character_storage_list(const int fd, struct map_session_data *sd,
                                  const char *, const char *message)
{
    struct storage *stor;
    struct map_session_data *pl_sd;
    struct item_data *item_data, *item_temp;
    int i, j, count, counter, counter2;
    char character[100];

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charitemlist <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can look items only lower or same level
            if ((stor = account2storage2(pl_sd->status.account_id)) != NULL)
            {
                counter = 0;
                count = 0;
                for (i = 0; i < MAX_STORAGE; i++)
                {
                    if (stor->storage_[i].nameid > 0
                        && (item_data =
                            itemdb_search(stor->storage_[i].nameid)) != NULL)
                    {
                        counter = counter + stor->storage_[i].amount;
                        count++;
                        if (count == 1)
                        {
                            std::string output = STRPRINTF(
                                     "------ Storage items list of '%s' ------",
                                     pl_sd->status.name);
                            clif_displaymessage(fd, output);
                        }
                        std::string output;
                        if (stor->storage_[i].refine)
                            output = STRPRINTF("%d %s %+d (%s %+d, id: %d)",
                                     stor->storage_[i].amount,
                                     item_data->name,
                                     stor->storage_[i].refine,
                                     item_data->jname,
                                     stor->storage_[i].refine,
                                     stor->storage_[i].nameid);
                        else
                            output = STRPRINTF("%d %s (%s, id: %d)",
                                     stor->storage_[i].amount,
                                     item_data->name, item_data->jname,
                                     stor->storage_[i].nameid);
                        clif_displaymessage(fd, output);

                        output.clear();
                        counter2 = 0;
                        for (j = 0; j < item_data->slot; j++)
                        {
                            if (stor->storage_[i].card[j])
                            {
                                if ((item_temp =
                                     itemdb_search(stor->
                                                    storage_[i].card[j])) !=
                                    NULL)
                                {
                                    if (output.empty())
                                        output = STRPRINTF(
                                                " -> (card(s): #%d %s (%s), ",
                                                ++counter2,
                                                item_temp->name,
                                                item_temp->jname);
                                    else
                                        output += STRPRINTF(
                                                "#%d %s (%s), ",
                                                ++counter2,
                                                item_temp->name,
                                                item_temp->jname);
                                }
                            }
                        }
                        if (!output.empty())
                        {
                            // replace last ", "
#ifdef WORKAROUND_GCC46_LIBRARY
                            output.resize(output.size() - 1);
#else
                            output.pop_back();
#endif
                            output.back() = ')';
                            clif_displaymessage(fd, output);
                        }
                    }
                }
                if (count == 0)
                    clif_displaymessage(fd,
                                         "No item found in the storage of this player.");
                else
                {
                    std::string output = STRPRINTF(
                             "%d item(s) found in %d kind(s) of items.",
                             counter, count);
                    clif_displaymessage(fd, output);
                }
            }
            else
            {
                clif_displaymessage(fd, "This player has no storage.");
                return -1;
            }
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @charcartlist <character>: Displays the items list of a player's cart.
 *------------------------------------------
 */
int atcommand_character_cart_list(const int fd, struct map_session_data *sd,
                               const char *, const char *message)
{
    struct map_session_data *pl_sd;
    struct item_data *item_data, *item_temp;
    int i, j, count, counter, counter2;
    char character[100];

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd,
                             "Please, enter a player name (usage: @charitemlist <char name>).");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can look items only lower or same level
            counter = 0;
            count = 0;
            for (i = 0; i < MAX_CART; i++)
            {
                if (pl_sd->status.cart[i].nameid > 0
                    && (item_data =
                        itemdb_search(pl_sd->status.cart[i].nameid)) != NULL)
                {
                    counter = counter + pl_sd->status.cart[i].amount;
                    count++;
                    if (count == 1)
                    {
                        std::string output = STRPRINTF(
                                "------ Cart items list of '%s' ------",
                                pl_sd->status.name);
                        clif_displaymessage(fd, output);
                    }

                    std::string output;
                    if (pl_sd->status.cart[i].refine)
                        output = STRPRINTF("%d %s %+d (%s %+d, id: %d)",
                                pl_sd->status.cart[i].amount,
                                item_data->name,
                                pl_sd->status.cart[i].refine,
                                item_data->jname,
                                pl_sd->status.cart[i].refine,
                                pl_sd->status.cart[i].nameid);
                    else

                        output = STRPRINTF("%d %s (%s, id: %d)",
                                pl_sd->status.cart[i].amount,
                                item_data->name, item_data->jname,
                                pl_sd->status.cart[i].nameid);
                    clif_displaymessage(fd, output);

                    output.clear();
                    counter2 = 0;
                    for (j = 0; j < item_data->slot; j++)
                    {
                        if (pl_sd->status.cart[i].card[j])
                        {
                            if ((item_temp =
                                 itemdb_search(pl_sd->status.
                                                cart[i].card[j])) != NULL)
                            {
                                if (output.empty())
                                    output = STRPRINTF(
                                            " -> (card(s): #%d %s (%s), ",
                                            ++counter2,
                                            item_temp->name,
                                            item_temp->jname);
                                else
                                    output += STRPRINTF(
                                            "#%d %s (%s), ",
                                            ++counter2,
                                            item_temp->name,
                                            item_temp->jname);
                            }
                        }
                    }
                    if (!output.empty())
                    {
#ifdef WORKAROUND_GCC46_LIBRARY
                        output.resize(output.size() - 1);
#else
                        output.pop_back();
#endif
                        output.back() = '0';
                        clif_displaymessage(fd, output);
                    }
                }
            }
            if (count == 0)
                clif_displaymessage(fd,
                                     "No item found in the cart of this player.");
            else
            {
                std::string output = STRPRINTF("%d item(s) found in %d kind(s) of items.",
                         counter, count);
                clif_displaymessage(fd, output);
            }
        }
        else
        {
            clif_displaymessage(fd, "Your GM level don't authorise you to do this action on this player.");
            return -1;
        }
    }
    else
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    return 0;
}

/*==========================================
 * @killer by MouseJstr
 * enable killing players even when not in pvp
 *------------------------------------------
 */
int atcommand_killer(const int fd, struct map_session_data *sd,
                  const char *, const char *)
{
    sd->special_state.killer = !sd->special_state.killer;

    if (sd->special_state.killer)
        clif_displaymessage(fd, "You be a killa...");
    else
        clif_displaymessage(fd, "You gonna be own3d...");

    return 0;
}

/*==========================================
 * @killable by MouseJstr
 * enable other people killing you
 *------------------------------------------
 */
int atcommand_killable(const int fd, struct map_session_data *sd,
                    const char *, const char *)
{
    sd->special_state.killable = !sd->special_state.killable;

    if (sd->special_state.killable)
        clif_displaymessage(fd, "You gonna be own3d...");
    else
        clif_displaymessage(fd, "You be a killa...");

    return 0;
}

/*==========================================
 * @charkillable by MouseJstr
 * enable another player to be killed
 *------------------------------------------
 */
int atcommand_charkillable(const int fd, struct map_session_data *,
                        const char *, const char *message)
{
    struct map_session_data *pl_sd = NULL;

    if (!message || !*message)
        return -1;

    if ((pl_sd = map_nick2sd(message)) == NULL)
        return -1;

    pl_sd->special_state.killable = !pl_sd->special_state.killable;

    if (pl_sd->special_state.killable)
        clif_displaymessage(fd, "The player is now killable");
    else
        clif_displaymessage(fd, "The player is no longer killable");

    return 0;
}

/*==========================================
 * @npcmove by MouseJstr
 *
 * move a npc
 *------------------------------------------
 */
int atcommand_npcmove(const int, struct map_session_data *sd,
                   const char *, const char *message)
{
    char character[100];
    int x = 0, y = 0;
    struct npc_data *nd = 0;

    if (sd == NULL)
        return -1;

    if (!message || !*message)
        return -1;

    memset(character, '\0', sizeof character);

    if (sscanf(message, "%d %d %99[^\n]", &x, &y, character) < 3)
        return -1;

    nd = npc_name2id(character);
    if (nd == NULL)
        return -1;

    npc_enable(character, 0);
    nd->bl.x = x;
    nd->bl.y = y;
    npc_enable(character, 1);

    return 0;
}

/*==========================================
 * @addwarp by MouseJstr
 *
 * Create a new static warp point.
 *------------------------------------------
 */
int atcommand_addwarp(const int fd, struct map_session_data *sd,
                   const char *, const char *message)
{
    char mapname[30];
    int x, y, ret;

    if (!message || !*message)
        return -1;

    if (sscanf(message, "%29s %d %d[^\n]", mapname, &x, &y) < 3)
        return -1;

    std::string w1 = STRPRINTF("%s,%d,%d", sd->mapname, sd->bl.x, sd->bl.y);
    std::string w3 = STRPRINTF("%s%d%d%d%d", mapname, sd->bl.x, sd->bl.y, x, y);
    std::string w4 = STRPRINTF("1,1,%s.gat,%d,%d", mapname, x, y);

    ret = npc_parse_warp(w1.c_str(), "warp", w3.c_str(), w4.c_str());

    std::string output = STRPRINTF("New warp NPC => %s", w3);
    clif_displaymessage(fd, output);

    return ret;
}

/*==========================================
 * @chareffect by [MouseJstr]
 *
 * Create a effect localized on another character
 *------------------------------------------
 */
int atcommand_chareffect(const int fd, struct map_session_data *,
                      const char *, const char *message)
{
    struct map_session_data *pl_sd = NULL;
    char target[255];
    int type = 0;

    if (!message || !*message
        || sscanf(message, "%d %s", &type, target) != 2)
    {
        clif_displaymessage(fd, "usage: @chareffect <type+> <target>.");
        return -1;
    }

    if ((pl_sd = map_nick2sd(target)) == NULL)
        return -1;

    clif_specialeffect(&pl_sd->bl, type, 0);
    clif_displaymessage(fd, "Your Effect Has Changed.");   // Your effect has changed.

    return 0;
}

/*==========================================
 * @dropall by [MouseJstr]
 *
 * Drop all your possession on the ground
 *------------------------------------------
 */
int atcommand_dropall(const int, struct map_session_data *sd,
                   const char *, const char *)
{
    int i;
    for (i = 0; i < MAX_INVENTORY; i++)
    {
        if (sd->status.inventory[i].amount)
        {
            if (bool(sd->status.inventory[i].equip))
                pc_unequipitem(sd, i, CalcStatus::NOW);
            pc_dropitem(sd, i, sd->status.inventory[i].amount);
        }
    }
    return 0;
}

/*==========================================
 * @chardropall by [MouseJstr]
 *
 * Throw all the characters possessions on the ground.  Normally
 * done in response to them being disrespectful of a GM
 *------------------------------------------
 */
int atcommand_chardropall(const int fd, struct map_session_data *,
                       const char *, const char *message)
{
    int i;
    struct map_session_data *pl_sd = NULL;

    if (!message || !*message)
        return -1;
    if ((pl_sd = map_nick2sd(message)) == NULL)
        return -1;
    for (i = 0; i < MAX_INVENTORY; i++)
    {
        if (pl_sd->status.inventory[i].amount)
        {
            if (bool(pl_sd->status.inventory[i].equip))
                pc_unequipitem(pl_sd, i, CalcStatus::NOW);
            pc_dropitem(pl_sd, i, pl_sd->status.inventory[i].amount);
        }
    }

    clif_displaymessage(pl_sd->fd, "Ever play 52 card pickup?");
    clif_displaymessage(fd, "It is done");
    //clif_displaymessage(fd, "It is offical.. your a jerk");

    return 0;
}

/*==========================================
 * @storeall by [MouseJstr]
 *
 * Put everything into storage to simplify your inventory to make
 * debugging easie
 *------------------------------------------
 */
int atcommand_storeall(const int fd, struct map_session_data *sd,
                    const char *, const char *)
{
    int i;
    nullpo_retr(-1, sd);

    if (!sd->state.storage_open)
    {                           //Open storage.
        switch (storage_storageopen(sd))
        {
            case 2:            //Try again
                clif_displaymessage(fd, "run this command again..");
                return 0;
            case 1:            //Failure
                clif_displaymessage(fd,
                                     "You can't open the storage currently.");
                return 1;
        }
    }
    for (i = 0; i < MAX_INVENTORY; i++)
    {
        if (sd->status.inventory[i].amount)
        {
            if (bool(sd->status.inventory[i].equip))
                pc_unequipitem(sd, i, CalcStatus::NOW);
            storage_storageadd(sd, i, sd->status.inventory[i].amount);
        }
    }
    storage_storageclose(sd);

    clif_displaymessage(fd, "It is done");
    return 0;
}

/*==========================================
 * @charstoreall by [MouseJstr]
 *
 * A way to screw with players who piss you off
 *------------------------------------------
 */
int atcommand_charstoreall(const int fd, struct map_session_data *sd,
                        const char *, const char *message)
{
    int i;
    struct map_session_data *pl_sd = NULL;

    if (!message || !*message)
        return -1;
    if ((pl_sd = map_nick2sd(message)) == NULL)
        return -1;

    if (storage_storageopen(pl_sd) == 1)
    {
        clif_displaymessage(fd,
                             "Had to open the characters storage window...");
        clif_displaymessage(fd, "run this command again..");
        return 0;
    }
    for (i = 0; i < MAX_INVENTORY; i++)
    {
        if (pl_sd->status.inventory[i].amount)
        {
            if (bool(pl_sd->status.inventory[i].equip))
                pc_unequipitem(pl_sd, i, CalcStatus::NOW);
            storage_storageadd(pl_sd, i, sd->status.inventory[i].amount);
        }
    }
    storage_storageclose(pl_sd);

    clif_displaymessage(pl_sd->fd,
                         "Everything you own has been put away for safe keeping.");
    clif_displaymessage(pl_sd->fd,
                         "go to the nearest kafka to retrieve it..");
    clif_displaymessage(pl_sd->fd, "   -- the management");

    clif_displaymessage(fd, "It is done");

    return 0;
}

/*==========================================
 * It is made to rain.
 *------------------------------------------
 */
int atcommand_rain(const int, struct map_session_data *sd,
                const char *, const char *)
{
    int effno = 0;
    effno = 161;
    nullpo_retr(-1, sd);
    if (effno < 0 || map[sd->bl.m].flag.rain)
        return -1;

    map[sd->bl.m].flag.rain = 1;
    clif_specialeffect(&sd->bl, effno, 2);
    return 0;
}

/*==========================================
 * It is made to snow.
 *------------------------------------------
 */
int atcommand_snow(const int, struct map_session_data *sd,
                const char *, const char *)
{
    int effno = 0;
    effno = 162;
    nullpo_retr(-1, sd);
    if (effno < 0 || map[sd->bl.m].flag.snow)
        return -1;

    map[sd->bl.m].flag.snow = 1;
    clif_specialeffect(&sd->bl, effno, 2);
    return 0;
}

/*==========================================
 * Cherry tree snowstorm is made to fall. (Sakura)
 *------------------------------------------
 */
int atcommand_sakura(const int, struct map_session_data *sd,
                  const char *, const char *)
{
    int effno = 0;
    effno = 163;
    nullpo_retr(-1, sd);
    if (effno < 0 || map[sd->bl.m].flag.sakura)
        return -1;

    map[sd->bl.m].flag.sakura = 1;
    clif_specialeffect(&sd->bl, effno, 2);
    return 0;
}

/*==========================================
 * Fog hangs over.
 *------------------------------------------
 */
int atcommand_fog(const int, struct map_session_data *sd,
               const char *, const char *)
{
    int effno = 0;
    effno = 233;
    nullpo_retr(-1, sd);
    if (effno < 0 || map[sd->bl.m].flag.fog)
        return -1;

    map[sd->bl.m].flag.fog = 1;
    clif_specialeffect(&sd->bl, effno, 2);

    return 0;
}

/*==========================================
 * Fallen leaves fall.
 *------------------------------------------
 */
int atcommand_leaves(const int, struct map_session_data *sd,
                  const char *, const char *)
{
    int effno = 0;
    effno = 333;
    nullpo_retr(-1, sd);
    if (effno < 0 || map[sd->bl.m].flag.leaves)
        return -1;

    map[sd->bl.m].flag.leaves = 1;
    clif_specialeffect(&sd->bl, effno, 2);
    return 0;
}

/*==========================================
 *
 *------------------------------------------
 */
int atcommand_summon(const int, struct map_session_data *sd,
                      const char *, const char *message)
{
    char name[100];
    int mob_id = 0;
    int x = 0;
    int y = 0;
    int id = 0;
    struct mob_data *md;
    tick_t tick = gettick();

    nullpo_retr(-1, sd);

    if (!message || !*message)
        return -1;
    if (sscanf(message, "%99[^\n]", name) < 1)
        return -1;

    if ((mob_id = atoi(name)) == 0)
        mob_id = mobdb_searchname(name);
    if (mob_id == 0)
        return -1;

    x = sd->bl.x + random_::in(-5, 4);
    y = sd->bl.y + random_::in(-5, 4);

    id = mob_once_spawn(sd, "this", x, y, "--ja--", mob_id, 1, "");
    if ((md = (struct mob_data *) map_id2bl(id)))
    {
        md->master_id = sd->bl.id;
        md->state.special_mob_ai = 1;
        md->mode = mob_db[md->mob_class].mode | MobMode::AGGRESSIVE;
        md->deletetimer = add_timer(tick + std::chrono::minutes(1),
                std::bind(mob_timer_delete, ph::_1, ph::_2,
                    id));
        clif_misceffect(&md->bl, 344);
    }

    return 0;
}

/*==========================================
 * @adjcmdlvl by [MouseJstr]
 *
 * Temp adjust the GM level required to use a GM command
 *
 * Used during beta testing to allow players to use GM commands
 * for short periods of time
 *------------------------------------------
 */
int atcommand_adjcmdlvl(const int fd, struct map_session_data *,
                     const char *, const char *message)
{
    int i, newlev;
    char cmd[100];

    if (!message || !*message || sscanf(message, "%d %s", &newlev, cmd) != 2)
    {
        clif_displaymessage(fd, "usage: @adjcmdlvl <lvl> <command>.");
        return -1;
    }

    for (i = 0; atcommand_info[i].command; i++)
        if (strcasecmp(cmd, atcommand_info[i].command + 1) == 0)
        {
            atcommand_info[i].level = newlev;
            clif_displaymessage(fd, "@command level changed.");
            return 0;
        }

    clif_displaymessage(fd, "@command not found.");
    return -1;
}

/*==========================================
 * @adjgmlvl by [MouseJstr]
 *
 * Create a temp GM
 *
 * Used during beta testing to allow players to use GM commands
 * for short periods of time
 *------------------------------------------
 */
int atcommand_adjgmlvl(const int fd, struct map_session_data *,
                    const char *, const char *message)
{
    int newlev;
    char user[100];
    struct map_session_data *pl_sd;

    if (!message || !*message
        || sscanf(message, "%d %s", &newlev, user) != 2)
    {
        clif_displaymessage(fd, "usage: @adjgmlvl <lvl> <user>.");
        return -1;
    }

    if ((pl_sd = map_nick2sd(user)) == NULL)
        return -1;

    pc_set_gm_level(pl_sd->status.account_id, newlev);

    return 0;
}

/*==========================================
 * @trade by [MouseJstr]
 *
 * Open a trade window with a remote player
 *
 * If I have to jump to a remote player one more time, I am
 * gonna scream!
 *------------------------------------------
 */
int atcommand_trade(const int, struct map_session_data *sd,
                 const char *, const char *message)
{
    struct map_session_data *pl_sd = NULL;

    if (!message || !*message)
        return -1;
    if ((pl_sd = map_nick2sd(message)) != NULL)
    {
        trade_traderequest(sd, pl_sd->bl.id);
        return 0;
    }
    return -1;
}

/* Magic atcommands by Fate */

static
SkillID magic_skills[] =
{
    SkillID::TMW_MAGIC,
    SkillID::TMW_MAGIC_LIFE,
    SkillID::TMW_MAGIC_WAR,
    SkillID::TMW_MAGIC_TRANSMUTE,
    SkillID::TMW_MAGIC_NATURE,
    SkillID::TMW_MAGIC_ETHER,
};

constexpr
size_t magic_skills_nr = sizeof(magic_skills) / sizeof(magic_skills[0]);

static
const char *magic_skill_names[magic_skills_nr] =
{
    "magic",
    "life",
    "war",
    "transmute",
    "nature",
    "astral"
};

int atcommand_magic_info(const int fd, struct map_session_data *,
                      const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd, "Usage: @magicinfo <char_name>");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        std::string buf = STRPRINTF(
                "`%s' has the following magic skills:",
                character);
        clif_displaymessage(fd, buf);

        for (size_t i = 0; i < magic_skills_nr; i++)
        {
            SkillID sk = magic_skills[i];
            buf = STRPRINTF(
                    "%d in %s",
                    pl_sd->status.skill[sk].lv,
                    magic_skill_names[i]);
            if (pl_sd->status.skill[sk].lv)
                clif_displaymessage(fd, buf);
        }

        return 0;
    }
    else
        clif_displaymessage(fd, "Character not found.");

    return -1;
}

static
void set_skill(struct map_session_data *sd, SkillID i, int level)
{
    sd->status.skill[i].lv = level;
}

int atcommand_set_magic(const int fd, struct map_session_data *,
                     const char *, const char *message)
{
    char character[100];
    char magic_type[20];
    int value;
    struct map_session_data *pl_sd;

    memset(character, '\0', sizeof(character));

    if (!message || !*message
        || sscanf(message, "%19s %i %99[^\n]", magic_type, &value,
                   character) < 1)
    {
        clif_displaymessage(fd,
                             "Usage: @setmagic <school> <value> <char-name>, where <school> is either `magic', one of the school names, or `all'.");
        return -1;
    }

    SkillID skill_index = SkillID::NEGATIVE;
    if (!strcasecmp("all", magic_type))
        skill_index = SkillID::ZERO;
    else
    {
        for (size_t i = 0; i < magic_skills_nr; i++)
        {
            if (!strcasecmp(magic_skill_names[i], magic_type))
            {
                skill_index = magic_skills[i];
                break;
            }
        }
    }

    if (skill_index == SkillID::NEGATIVE)
    {
        clif_displaymessage(fd,
                             "Incorrect school of magic.  Use `magic', `nature', `life', `war', `transmute', `ether', or `all'.");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (skill_index == SkillID::ZERO)
            for (SkillID sk : magic_skills)
                set_skill(pl_sd, sk, value);
        else
            set_skill(pl_sd, skill_index, value);

        clif_skillinfoblock(pl_sd);
        return 0;
    }
    else
        clif_displaymessage(fd, "Character not found.");

    return -1;
}

int atcommand_log(const int, struct map_session_data *,
               const char *, const char *)
{
    return 0;
    // only used for (implicit) logging
}

int atcommand_tee(const int, struct map_session_data *sd,
               const char *, const char *message)
{
    char data[strlen(message) + 28];
    strcpy(data, sd->status.name);
    strcat(data, " : ");
    strcat(data, message);
    clif_message(&sd->bl, data);
    return 0;
}

int atcommand_invisible(const int, struct map_session_data *sd,
                     const char *, const char *)
{
    pc_invisibility(sd, 1);
    return 0;
}

int atcommand_visible(const int, struct map_session_data *sd,
                   const char *, const char *)
{
    pc_invisibility(sd, 0);
    return 0;
}

static
int atcommand_jump_iterate(const int fd, struct map_session_data *sd,
        const char *, const char *,
        struct map_session_data *(*get_start)(void),
        struct map_session_data *(*get_next)(struct map_session_data*))
{
    struct map_session_data *pl_sd;

    pl_sd = (struct map_session_data *) map_id2bl(sd->followtarget);

    if (pl_sd)
        pl_sd = get_next(pl_sd);

    if (!pl_sd)
        pl_sd = get_start();

    if (pl_sd == sd)
    {
        pl_sd = get_next(pl_sd);
        if (!pl_sd)
            pl_sd = get_start();
    }

    if (pl_sd->bl.m >= 0 && map[pl_sd->bl.m].flag.nowarpto
        && battle_config.any_warp_GM_min_level > pc_isGM(sd))
    {
        clif_displaymessage(fd,
                             "You are not authorised to warp you to the map of this player.");
        return -1;
    }
    if (sd->bl.m >= 0 && map[sd->bl.m].flag.nowarp
        && battle_config.any_warp_GM_min_level > pc_isGM(sd))
    {
        clif_displaymessage(fd,
                             "You are not authorised to warp you from your actual map.");
        return -1;
    }
    pc_setpos(sd, map[pl_sd->bl.m].name, pl_sd->bl.x, pl_sd->bl.y, BeingRemoveWhy::WARPED);
    std::string output = STRPRINTF("Jump to %s", pl_sd->status.name);
    clif_displaymessage(fd, output);

    sd->followtarget = pl_sd->bl.id;

    return 0;
}

int atcommand_iterate_forward_over_players(const int fd,
                                        struct map_session_data *sd,
                                        const char *command,
                                        const char *message)
{
    return atcommand_jump_iterate(fd, sd, command, message,
                                   map_get_first_session,
                                   map_get_next_session);
}

int atcommand_iterate_backwards_over_players(const int fd,
                                          struct map_session_data *sd,
                                          const char *command,
                                          const char *message)
{
    return atcommand_jump_iterate(fd, sd, command, message,
                                   map_get_last_session,
                                   map_get_prev_session);
}

int atcommand_wgm(const int fd, struct map_session_data *sd,
                   const char *, const char *message)
{
    if (tmw_CheckChatSpam(sd, message))
        return 0;

    tmw_GmHackMsg(static_cast<const std::string&>(STRPRINTF("[GM] %s: %s", sd->status.name, message)));
    if (!pc_isGM(sd))
        clif_displaymessage(fd, "Message sent.");

    return 0;
}


int atcommand_skillpool_info(const int fd, struct map_session_data *,
                              const char *, const char *message)
{
    char character[100];
    struct map_session_data *pl_sd;

    if (!message || !*message || sscanf(message, "%99[^\n]", character) < 1)
    {
        clif_displaymessage(fd, "Usage: @sp-info <char_name>");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        SkillID pool_skills[MAX_SKILL_POOL];
        int pool_skills_nr = skill_pool(pl_sd, pool_skills);
        int i;

        std::string buf = STRPRINTF(
                "Active skills %d out of %d for %s:",
                pool_skills_nr, skill_pool_max(pl_sd), character);
        clif_displaymessage(fd, buf);
        for (i = 0; i < pool_skills_nr; ++i)
        {
            buf = STRPRINTF(" - %s [%d]: power %d",
                    skill_name(pool_skills[i]),
                    pool_skills[i],
                    skill_power(pl_sd, pool_skills[i]));
            clif_displaymessage(fd, buf);
        }

        buf = STRPRINTF("Learned skills out of %d for %s:",
                skill_pool_skills_size, character);
        clif_displaymessage(fd, buf);

        for (i = 0; i < skill_pool_skills_size; ++i)
        {
            const char *name = skill_name(skill_pool_skills[i]);
            int lvl = pl_sd->status.skill[skill_pool_skills[i]].lv;

            if (lvl)
            {
                buf = STRPRINTF(" - %s [%d]: lvl %d",
                        name, skill_pool_skills[i], lvl);
                clif_displaymessage(fd, buf);
            }
        }

    }
    else
        clif_displaymessage(fd, "Character not found.");

    return 0;
}

int atcommand_skillpool_focus(const int fd, struct map_session_data *,
                               const char *, const char *message)
{
    char character[100];
    int skill_;
    struct map_session_data *pl_sd;

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &skill_, character) < 1)
    {
        clif_displaymessage(fd, "Usage: @sp-focus <skill-nr> <char_name>");
        return -1;
    }

    SkillID skill = SkillID(skill_);

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (skill_pool_activate(pl_sd, skill))
            clif_displaymessage(fd, "Activation failed.");
        else
            clif_displaymessage(fd, "Activation successful.");
    }
    else
        clif_displaymessage(fd, "Character not found.");

    return 0;
}

int atcommand_skillpool_unfocus(const int fd, struct map_session_data *,
                                 const char *, const char *message)
{
    char character[100];
    int skill_;
    struct map_session_data *pl_sd;

    if (!message || !*message
        || sscanf(message, "%d %99[^\n]", &skill_, character) < 1)
    {
        clif_displaymessage(fd, "Usage: @sp-unfocus <skill-nr> <char_name>");
        return -1;
    }

    SkillID skill = SkillID(skill_);

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        if (skill_pool_deactivate(pl_sd, skill))
            clif_displaymessage(fd, "Deactivation failed.");
        else
            clif_displaymessage(fd, "Deactivation successful.");
    }
    else
        clif_displaymessage(fd, "Character not found.");

    return 0;
}

int atcommand_skill_learn(const int fd, struct map_session_data *,
                           const char *, const char *message)
{
    char character[100];
    int skill_, level;
    struct map_session_data *pl_sd;

    if (!message || !*message
        || sscanf(message, "%d %d %99[^\n]", &skill_, &level, character) < 1)
    {
        clif_displaymessage(fd,
                             "Usage: @skill-learn <skill-nr> <level> <char_name>");
        return -1;
    }

    SkillID skill = SkillID(skill_);

    if ((pl_sd = map_nick2sd(character)) != NULL)
    {
        set_skill(pl_sd, skill, level);
        clif_skillinfoblock(pl_sd);
    }
    else
        clif_displaymessage(fd, "Character not found.");

    return 0;
}

int atcommand_ipcheck(const int fd, struct map_session_data *,
                   const char *, const char *message)
{
    struct map_session_data *pl_sd;
    struct sockaddr_in sai;
    char character[25];
    int i;
    socklen_t sa_len = sizeof(struct sockaddr);
    unsigned long ip;

    memset(character, '\0', sizeof(character));

    if (sscanf(message, "%24[^\n]", character) < 1)
    {
        clif_displaymessage(fd, "Usage: @ipcheck <char name>");
        return -1;
    }

    if ((pl_sd = map_nick2sd(character)) == NULL)
    {
        clif_displaymessage(fd, "Character not found.");
        return -1;
    }

    if (getpeername(pl_sd->fd, (struct sockaddr *)&sai, &sa_len))
    {
        clif_displaymessage(fd,
                             "Guru Meditation Error: getpeername() failed");
        return -1;
    }

    ip = sai.sin_addr.s_addr;

    // We now have the IP address of a character.
    // Loop over all logged in sessions looking for matches.

    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth)
        {
            if (getpeername(pl_sd->fd, (struct sockaddr *)&sai, &sa_len))
                continue;

            // Is checking GM levels really needed here?
            if (ip == sai.sin_addr.s_addr)
            {
                std::string output = STRPRINTF(
                        "Name: %s | Location: %s %d %d",
                        pl_sd->status.name, pl_sd->mapname,
                        pl_sd->bl.x, pl_sd->bl.y);
                clif_displaymessage(fd, output);
            }
        }
    }

    clif_displaymessage(fd, "End of list");
    return 0;
}

int atcommand_doomspot(const int fd, struct map_session_data *sd,
                       const char *, const char *)
{
    struct map_session_data *pl_sd;
    int i;

    for (i = 0; i < fd_max; i++)
    {
        if (session[i] && (pl_sd = (struct map_session_data *)session[i]->session_data)
            && pl_sd->state.auth && i != fd && sd->bl.m == pl_sd->bl.m
            && sd->bl.x == pl_sd->bl.x && sd->bl.y == pl_sd->bl.y
            && pc_isGM(sd) >= pc_isGM(pl_sd))
        {                       // you can doom only lower or same gm level
            pc_damage(NULL, pl_sd, pl_sd->status.hp + 1);
            clif_displaymessage(pl_sd->fd, "The holy messenger has given judgement.");
        }
    }
    clif_displaymessage(fd, "Judgement was made.");

    return 0;
}
