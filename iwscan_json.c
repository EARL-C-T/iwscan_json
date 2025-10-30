
#include "iwscan_json.h"

int jscn_param_json(iwparam parm, cJSON *obj) {
  cJSON_AddNumberToObject(obj, "value", parm.value);
  cJSON_AddNumberToObject(obj, "fixed", parm.fixed);
  cJSON_AddNumberToObject(obj, "disable", parm.fixed);
  cJSON_AddNumberToObject(obj, "flags", parm.flags);
  return 0;
}
cJSON *jscn_b_json(wireless_config b) {
  cJSON *config = cJSON_CreateObject();
  cJSON *essid = cJSON_CreateObject();
  cJSON_AddNumberToObject(essid, "has", b.has_essid);
  cJSON_AddStringToObject(essid, "essid", b.essid);
  cJSON_AddNumberToObject(essid, "len", b.essid_len);
  cJSON_AddNumberToObject(essid, "on", b.essid_on);
  cJSON_AddItemToObject(config, "essid", essid);
  cJSON *freq = cJSON_CreateObject();
  cJSON_AddNumberToObject(freq, "has", b.has_freq);
  cJSON_AddNumberToObject(freq, "freq", b.freq);
  cJSON_AddNumberToObject(freq, "flags", b.freq_flags);
  cJSON_AddItemToObject(config, "freq", freq);
  cJSON *key = cJSON_CreateObject();
  cJSON_AddNumberToObject(key, "has", b.has_key);
  char keybuff[b.key_size + 1];
  iw_print_key(keybuff, b.key_size + 1, b.key, b.key_size, b.key_flags);
  cJSON_AddStringToObject(key, "key", keybuff);
  cJSON_AddNumberToObject(key, "flags", b.key_flags);
  cJSON_AddNumberToObject(key, "len", b.key_size);
  cJSON_AddItemToObject(config, "key", key);
  return config;
}
cJSON *jscn_stats_json(iwstats s) {
  cJSON *stats = cJSON_CreateObject();
  cJSON *qual = cJSON_CreateObject();
  cJSON_AddNumberToObject(qual, "quality", s.qual.qual);
  cJSON_AddNumberToObject(qual, "dbm", s.qual.level);
  cJSON_AddNumberToObject(qual, "noise", s.qual.noise);
  cJSON_AddNumberToObject(qual, "flags", s.qual.updated);
  cJSON_AddItemToObject(stats, "stats", stats);
  cJSON *disflag = cJSON_CreateObject();
  cJSON_AddNumberToObject(disflag, "essid", s.discard.nwid);
  cJSON_AddNumberToObject(disflag, "code", s.discard.code);
  cJSON_AddNumberToObject(disflag, "frag", s.discard.fragment);
  cJSON_AddNumberToObject(disflag, "ret", s.discard.retries);
  cJSON_AddNumberToObject(disflag, "misc", s.discard.misc);
  cJSON_AddItemToObject(stats, "disflag", disflag);
  cJSON_AddNumberToObject(stats, "status", s.status);
  cJSON_AddNumberToObject(stats, "miss", s.miss.beacon);
  return stats;
}
cJSON *jscn_sckaddr_json(sockaddr s) {
  cJSON *skadd = cJSON_CreateObject();
  cJSON_AddNumberToObject(skadd, "sa_fam", s.sa_family);
  cJSON_AddStringToObject(skadd, "data", s.sa_data);
  return skadd;
}
cJSON *jscn_scn_json(wireless_scan w) {
  cJSON *scn = cJSON_CreateObject();
  cJSON_AddNumberToObject(scn, "has_ap_addr", w.has_ap_addr);
  cJSON *skadd = jscn_sckaddr_json(w.ap_addr);
  cJSON_AddItemToObject(scn, "ap_addr", skadd);
  cJSON *config = jscn_b_json(w.b);
  cJSON_AddItemToObject(scn, "config", config);
  cJSON_AddNumberToObject(scn, "has_stats", w.has_stats);
  cJSON *stats = jscn_stats_json(w.stats);
  cJSON_AddItemToObject(scn, "stats", stats);
  cJSON_AddNumberToObject(scn, "has_bitrate", w.has_maxbitrate);
  cJSON *mx_btrte = cJSON_CreateObject();
  jscn_param_json(w.maxbitrate, mx_btrte);
  cJSON_AddItemToObject(scn, "max_birates", mx_btrte);
  return scn;
}

cJSON *jscn_scnhd_json(wireless_scan_head shd) {
  cJSON *scan_rslts = cJSON_CreateObject();
  cJSON *scn_rslts = cJSON_CreateArray();
  wireless_scan *S = shd.result;
  int i = 0;
  while (NULL != S) {
    cJSON *scn = jscn_scn_json(*S);
    cJSON_AddItemToArray(scn_rslts, scn);
    S = S->next;
    i++;
  }
  cJSON_AddNumberToObject(scan_rslts, "scan_cnt", i);
  cJSON_AddItemToObject(scan_rslts, "resaults", scn_rslts);
  return scan_rslts;
}
cJSON *jscn_iwscan_json(char *ifname) {
  int skfd = socket(AF_INET, SOCK_STREAM, 0);
  int ver = iw_get_kernel_we_version();
  wireless_scan_head wsh;
  int k = iw_scan(skfd, ifname, ver, &wsh);
  if (k == 0) {
    cJSON *FULL = jscn_scnhd_json(wsh);
    cJSON_AddStringToObject(FULL, "ifname", ifname);
    return FULL;
  } else {
    int ERR = errno;
    exit(ERR);
  }
}
int jscn_print_file(char *fp, cJSON *scn) {
  FILE *f = fopen(fp, "w");
  if (f == NULL) {
    int ERR = errno;
    exit(ERR);
  }
  char *r = cJSON_Print(scn);
  fprint("%s\n", r);
  fputs(r, f);
  fclose(f);
  return 0;
}
