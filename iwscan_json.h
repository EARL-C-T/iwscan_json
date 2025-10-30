#ifndef IWSCAN_JSON_H_
#define IWSCAN_JSON_H_
/*iwscan_json
 *
 * EARL T [ET]
 *
 * I was still may make a lua iw binder but i decided that just
 * parsing iw scan output direct to json kinda makes more sense
 * as that is an extreamly portable format so from there you can
 * select your languge im sure there is a json parser for it
 * and parse the json or the json file then you can mess with
 * the data as you see fit the languge ya  want or  idk just use
 * jq make it look pretty idc all you
 */
#include <cjson/cJSON.h>
#include <errno.h>
#include <iwlib.h>
#include <stdio.h>
#include <stdlib.h>

/*this is for encoding any iwparam into an object supply pointer
 * to the cJSON obj you wish to use will return int but obj should
 * changed
 */

int jscn_param_json(iwparam parm, cJSON *obj);

/*this encodes wireless_config returns pointer to cJSON object
 * c.essid ,c.freq,c.key key should be in regular char array
 */

cJSON *jscn_b_json(wireless_config b);

/*this encodes iwstats  stats.qual ,stats.disflag, status and miss
 * are alson included
 */

cJSON *jscn_stats_json(iwstats s);

/*sock address sa_fam and data */

cJSON *jscn_sckaddr_json(sockaddr s);

/*this should do the whole wireless_scan struct */

cJSON *jscn_scn_json(wireless_scan w);

/*this dose the entire scan head im also gonna add a count int so you can look
 * at num results
 */

cJSON *jscn_scnhd_json(wireless_scan_head shd);

/*this dose morst the work only need to supply it with a char* ifname
 * and returns full scan results cJSON obect
 */

cJSON *jscn_iwscan_json(char *ifname);

/*this prints to file the only args are char fp path to file  and cJSON
 * pointer to cJSON Object could be anything is ment for the output of
 * jscn_iwscan_json
 */

int jscn_print_file(const char *fp, cJSON *scn);

#endif /*IWSCAN_JSON_H_*/
