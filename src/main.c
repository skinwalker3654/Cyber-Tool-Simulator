#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
    void sleep_ms(int ms) { Sleep(ms); }
    #define CLEAR "cls"
#else
    #include <unistd.h>
        void sleep_ms(int ms){ usleep(ms * 1000); }
    #define CLEAR "clear"
#endif

// ===== ANSI colors (RED vibe) =====
#define R   "\033[1;31m"   // bright red (alerts / negatives)
#define G   "\033[1;32m"   // green (OK/progress)
#define Y   "\033[1;33m"   // yellow (info/working)
#define B   "\033[0;31m"   // dark red (used where code had 'blue')
#define M   "\033[0;31m"   // map magenta to dark red
#define C   "\033[1;31m"   // cyan -> bright red (primary UI accent)
#define W   "\033[1;37m"   // white (content text)
#define DIM "\033[2m"      // dim/secondary
#define RS  "\033[0m"      // reset

void clear() { system(CLEAR); }
void type_effect(const char *s, int ms_per_char){
    for (size_t i=0; s[i]; ++i){
        putchar(s[i]);
        fflush(stdout);
        sleep_ms(ms_per_char);
    }
}

void header(const char *title){
    printf(C "┌───────────────────────────────────────────────────────────┐\n" RS);
    printf(C "│ CYBER SECURITY TOOL - HACKING TOOL                        │\n" RS);
    printf(C "├───────────────────────────────────────────────────────────┤\n" RS);
    printf(C "│ " W "%-57s" C " │\n" RS, title);
    printf(C "└───────────────────────────────────────────────────────────┘\n\n" RS);
}

void progress_bar(const char *label, int steps, int delay_ms){
    printf("%s%s%s\n", DIM, label, RS);
    int width = 40;
    for(int i=0;i<=steps;i++){
        int filled = (i*width)/steps;
        printf("\r[");
        for(int j=0;j<width;j++){
            if(j<filled) printf(R "█" RS);
            else printf(DIM "·" RS);
        }
        printf("] %3d%%", (i*100)/steps);
        fflush(stdout);
        sleep_ms(delay_ms);
    }
    printf("\n");
}

int rnd(int a, int b){ return a + rand() % (b - a + 1); }
void random_ip(char *buf, size_t n){
    snprintf(buf, n, "%d.%d.%d.%d", rnd(1,254), rnd(0,255), rnd(0,255), rnd(1,254));
}
void random_password(char *buf, size_t n) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789" "!@#$%^&*()-_=+[]{};:,.<>?/";
    size_t charset_size = sizeof(charset) - 1;
    for (size_t i = 0; i < n - 1; i++) {
        buf[i] = charset[rnd(0, charset_size - 1)];
    }
    buf[n - 1] = '\0';
}

void pause_enter(){
    printf(DIM "\n[Press Enter to return to menu]" RS);
    int c;
    while((c=getchar())!='\n' && c!=EOF){}
}

// ================= Modules =================
void mod_scan(){
    clear();
    header("NETWORK SCAN");
    type_effect(G "[+] Initiating passive scan...\n" RS, 10);
    sleep_ms(300);
    int found = rnd(4, 10);
    for(int i=0;i<found;i++){
        char ip[32];
        random_ip(ip, sizeof ip);
        printf(W " -> " C "Discovered host: " W "%s " DIM "(open ports: %d, filtered: %d)\n" RS, ip, rnd(1,6), rnd(0,3));
        sleep_ms(rnd(120, 280));
    }
    printf("\n");
    progress_bar("Fingerprinting services", 30, 40);
    printf(G "\n[OK] " W "Scan report saved to " C "reports/scan_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_bruteforce(){
    clear();
    header("PASSWORD BRUTEFORCE");
    char target[64];
    printf(W "Target username: " RS);
    fgets(target, sizeof target, stdin);
    target[strcspn(target, "\n")] = 0;
    if(strlen(target) == 0) strcpy(target, "admin");
    printf(Y "\n[*] " W "Loading wordlists, rules, masks...\n" RS);
    progress_bar("Preparing candidate space", 50, 20);
    int tries = rnd(1200, 2600);
    int reveal_at = rnd(200, tries-1);
    printf(C "\n[Session] " W "Mode: " C "dictionary+mask" W " • Keyspace: " C "%d" W " candidates\n\n" RS, tries);
    for(int i=1;i<=tries;i++){
        if(i%50==0) printf(DIM "\rTried %d/%d..." RS, i, tries), fflush(stdout);
        if(i==reveal_at){
            printf("\n" G "[+] " W "Password found for " C "%s" W ": " G "" RS, target);
            char pass[16]; random_password(pass,sizeof(pass));
            printf("%s\n",pass);
            printf(DIM "Hash: " RS "sha256:deadbeefcafebabe...\n");
            break;
        }
        sleep_ms(5);
    }
    printf(G "\n[OK] " W "Credentials stored at " C "loot/%s.creds\n" RS, target);
    pause_enter();
}

void mod_ddos(){
    clear();
    header("DDoS FLOODER");
    char host[64];
    printf(W "Target (IP/host): " RS);
    fgets(host, sizeof host, stdin);
    host[strcspn(host, "\n")] = 0;
    if(strlen(host) == 0) strcpy(host, "example.com");
    printf(Y "\n[*] " W "Spawning botnet nodes...\n" RS);
    int bots = rnd(200, 1200);
    progress_bar("Provisioning nodes", 35, 25);
    printf(C "\n[Info] " W "Active bots: " G "%d\n" RS, bots);
    printf(Y "[*] " W "Starting flood...\n" RS);
    for(int t=0; t<8; ++t){
        int pps = rnd(150000, 900000);
        int gbps = rnd(1, 80);
        printf(W " • Tick %d: " G "%d pps" W " • " G "%d Gbps" W " • status: " C "bypassing\n" RS, t+1, pps, gbps);
        sleep_ms(rnd(160, 300));
    }
    progress_bar("Sustaining pressure", 30, 35);
    printf(G "\n[OK] " W "Attack ended.\n" RS);
    pause_enter();
}

void mod_keylogger(){
    clear();
    header("KEYLOGGER VIEW");
    printf(DIM "Keyloger is starting taking infos from The Computer\n\n" RS);
    const char *samples[] = {
        "sudo apt update"
        , "hunter2"
        , "cd /var/www/html"
        , "ssh admin@10.0.0.7"
        , "password: ********"
        , "nano config.yml"
        , "export TOKEN=sk-********",
    };
    int n = sizeof(samples)/sizeof(samples[0]);
    for(int i=0;i<rnd(14, 22); ++i){
        const char *s = samples[rnd(0, n-1)];
        printf(C "[%02d:%02d:%02d] " W, rnd(0,23), rnd(0,59), rnd(0,59));
        for(size_t k=0; s[k]; ++k){
            putchar(s[k]);
            fflush(stdout);
            sleep_ms(rnd(12, 28));
        }
        printf("\n");
        sleep_ms(rnd(60, 140));
    }
    printf(G "\n[OK] " W "Stream ended.\n" RS);
    pause_enter();
}

void mod_email_breach(){
    clear();
    header("EMAIL BREACH SCAN");
    type_effect(G "[*] Scanning public databases for leaked credentials...\n" RS, 8);
    sleep_ms(500);
    const char *emails[] = {
        "alice@example.com"
        , "bob@mail.com"
        , "charlie@domain.org"
        , "admin@secure.net"
        , "testuser@web.io"
    };
    for(int i=0;i<rnd(5,10);i++){
        char pass[12];
        random_password(pass,sizeof(pass));
        printf(W " -> " C "Found: " W "%s " G "password: %s\n" RS, emails[rnd(0,4)], pass);
        sleep_ms(rnd(200,400));
    }
    progress_bar("Compiling report", 25, 25);
    printf(G "\n[OK] " W "Report saved to " C "loot/email_leaks_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_exploit_scan(){
    clear();
    header("EXPLOIT SCANNER");
    type_effect(G "[*] Scanning hosts for exploitable vulnerabilities...\n" RS, 8);
    sleep_ms(500);
    int hosts = rnd(3,7);
    for(int i=0;i<hosts;i++){
        char ip[32];
        random_ip(ip,sizeof(ip));
        printf(W " -> " C "Host: " W "%s " DIM "(open ports: %d, filtered: %d)\n" RS, ip, rnd(1,8), rnd(0,4));
        int vulns = rnd(1,5);
        for(int v=0;v<vulns;v++){
            printf(" " R "[-] Vulnerability found: " W "CVE-%d-%04d Severity: %s\n" RS, rnd(2000,2023), rnd(1000,9999), (rnd(0,1)?"HIGH":"MEDIUM"));
            sleep_ms(rnd(100,250));
        }
    }
    progress_bar("Generating exploit report", 30, 25);
    printf(G "\n[OK] " W "Report saved to " C "loot/exploit_report_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_ssh_brute(){
    clear(); header("SSH BRUTEFORCE");
    type_effect(Y "[*] Targeting SSH servers...\n" RS, 8);
    sleep_ms(500); int attempts = rnd(50,150);
    for(int i=1;i<=attempts;i++){
        char ip[32];
        random_ip(ip,sizeof(ip));
        char user[8];
        snprintf(user,sizeof(user),"user%d",rnd(1,99));
        printf(W "Attempt %3d: " C "%s@%s " DIM "-> " W "Login %s\n" RS, i, user, ip, (rnd(0,5)==0?"SUCCESS":"FAIL"));
        sleep_ms(rnd(80,200));
    }
    progress_bar("Collecting credentials", 20, 30);
    printf(G "\n[OK] " W "Credentials saved to " C "loot/ssh_creds_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_packet_sniffer(){
    clear();
    header("PACKET SNIFFER");
    type_effect(G "[*] Sniffing network traffic...\n" RS, 8);
    sleep_ms(500);
    for(int i=0;i<rnd(10,20);i++){
        char src[32], dst[32];
        random_ip(src,sizeof(src));
        random_ip(dst,sizeof(dst));
        printf(C "[%02d:%02d:%02d] " W "%s -> %s : " G "%d bytes captured\n" RS, rnd(0,23), rnd(0,59), rnd(0,59), src, dst, rnd(40,1500));
        sleep_ms(rnd(100,250));
    }
    progress_bar("Analyzing packets", 25, 20);
    printf(G "\n[OK] " W "Packet log saved to " C "loot/packets_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_crypto_scan(){
    clear();
    header("CRYPTO WALLET SCANNER");
    type_effect(G "[*] Searching blockchain wallets...\n" RS, 8);
    sleep_ms(500);
    for(int i=0;i<rnd(5,12);i++){
        char wallet[36];
        for(int j=0;j<34;j++) wallet[j] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"[rnd(0,57)];
        wallet[34]='\0';
        printf(W " -> " C "Wallet: " W "%s " G "Balance: %.4f BTC\n" RS, wallet, (double)rnd(1,10000)/1000);
        sleep_ms(rnd(200,400));
    }
    progress_bar("Compiling crypto report", 25, 25);
    printf(G "\n[OK] " W "Report saved to " C "loot/crypto_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

// ================= Extra Modules =================
void mod_sql_injection(){
    clear(); header("SQL INJECTION SCANNER");
    type_effect(G "[*] Testing web forms for SQL injection...\n" RS, 8);
    sleep_ms(500);
    int forms = rnd(3,6);
    for(int i=0;i<forms;i++){
        printf(W "Form %d: " C "Testing parameter 'user_input' ... " RS, i+1);
        sleep_ms(rnd(200,400));
        printf(G "VULNERABLE\n" RS);
    }
    progress_bar("Generating injection report", 30, 25);
    printf(G "\n[OK] " W "Report saved to " C "loot/sql_injection_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_wifi_cracker(){
    clear(); header("WIFI PASSWORD CRACKER");
    type_effect(Y "[*] Scanning nearby WiFi networks...\n" RS, 8);
    sleep_ms(500);
    int nets = rnd(3,8);
    for(int i=0;i<nets;i++){
        char ssid[16];
        snprintf(ssid,sizeof(ssid),"WiFi-%d",rnd(100,999));
        printf(W " -> Network: " C "%s " W "Password: " RS, ssid);
        char buff[16];
        random_password(buff,sizeof(buff));
        printf("%s\n",buff);
        sleep_ms(rnd(200,400));
    }
    progress_bar("Cracking WPA keys", 25, 30);
    printf(G "\n[OK] " W "Passwords saved to " C "loot/wifi_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_malware_dropper(){
    clear(); header("MALWARE DEPLOYER");
    type_effect(G "[*] Preparing payloads...\n" RS, 8);
    sleep_ms(500);
    int targets = rnd(2,5);
    for(int i=0;i<targets;i++){
        char host[32]; random_ip(host,sizeof(host));
        printf(W " -> Deploying payload to " C "%s ... " RS, host);
        sleep_ms(rnd(200,500));
        printf(G "SUCCESS\n" RS);
    }
    progress_bar("Payload distribution", 30, 20);
    printf(G "\n[OK] " W "Deployment logs saved to " C "loot/malware_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_stegano_scan(){
    clear(); header("STEGANOGRAPHY SCANNER");
    type_effect(G "[*] Scanning images for hidden data...\n" RS, 8);
    sleep_ms(500);
    int images = rnd(3,8);
    for(int i=0;i<images;i++){
        char img[16]; snprintf(img,sizeof(img),"img_%03d.png",rnd(100,999));
        printf(W " -> Scanning " C "%s " RS, img);
        sleep_ms(rnd(200,400));
        printf(G "Hidden content detected\n" RS);
    }
    progress_bar("Extracting hidden content", 25, 25);
    printf(G "\n[OK] " W "Report saved to " C "loot/stegano_%04d.log\n" RS, rnd(1000,9999));
    pause_enter();
}

void mod_ransomware(){
    clear(); header("RANSOMWARE EMULATOR");
    type_effect(R "[*] Encrypting files on target machine...\n" RS, 8);
    sleep_ms(500);
    int files = rnd(10,25);
    for(int i=0;i<files;i++){
        printf(W "Encrypting " C "file_%03d.txt ... " RS, i+1);
        sleep_ms(rnd(100,250));
        printf(G "DONE\n" RS);
    }
    progress_bar("Finalizing encryption", 25, 25);
    printf(R "\n[ALERT] " W "Files encrypted. Ransom note created at " C "loot/ransom_note_%04d.txt\n" RS, rnd(1000,9999));
    pause_enter();
}

// ================= Menu & Splash =================
void menu(){
    printf(B "┌─ " W "MENU" B " ─────────────────────────────┐\n" RS);
    printf(B "│ " W "1" B " ▸ " W "Scan Networks" RS); printf(B "                  │\n" RS);
    printf(B "│ " W "2" B " ▸ " W "Bruteforce Password" RS); printf(B "            │\n" RS);
    printf(B "│ " W "3" B " ▸ " W "DDoS Flood" RS); printf(B "                     │\n" RS);
    printf(B "│ " W "4" B " ▸ " W "Keylogger View" RS); printf(B "                 │\n" RS);
    printf(B "│ " W "5" B " ▸ " W "Email Breach Scan" RS); printf(B "              │\n" RS);
    printf(B "│ " W "6" B " ▸ " W "Exploit Scanner" RS); printf(B "                │\n" RS);
    printf(B "│ " W "7" B " ▸ " W "Packet Sniffer" RS); printf(B "                 │\n" RS);
    printf(B "│ " W "8" B " ▸ " W "SSH Brute Force" RS); printf(B "                │\n" RS);
    printf(B "│ " W "9" B " ▸ " W "Crypto Wallet Scanner" RS); printf(B "          │\n" RS);
    printf(B "│" W "10" B " ▸ " W "SQL Injection Scan" RS); printf(B "             │\n" RS);
    printf(B "│" W "11" B " ▸ " W "WiFi Password Cracker" RS); printf(B "          │\n" RS);
    printf(B "│" W "12" B " ▸ " W "Malware Deployer" RS); printf(B "               │\n" RS);
    printf(B "│" W "13" B " ▸ " W "Steganography Scan" RS); printf(B "             │\n" RS);
    printf(B "│" W "14" B " ▸ " W "Ransomware Emulator" RS); printf(B "            │\n" RS);
    printf(B "│" W "15" B " ▸ " W "Exit" RS); printf(B "                           │\n" RS);
    printf(B "└────────────────────────────────────┘\n\n" RS);
    printf(W "Select> " RS);
}

void splash(){
    clear();
    printf(C "\n");
    printf("███╗   ██╗███████╗████████╗████████╗ ██████╗ ██████╗ ███╗   ███╗\n");
    printf("████╗  ██║██╔════╝╚══██╔══╝╚══██╔══╝██╔═══██╗██╔══██╗████╗ ████║\n");
    printf("██╔██╗ ██║█████╗     ██║      ██║   ██║   ██║██████╔╝██╔████╔██║\n");
    printf("██║╚██╗██║██╔══╝     ██║      ██║   ██║   ██║██╔═══╝ ██║╚██╔╝██║\n");
    printf("██║ ╚████║███████╗   ██║      ██║   ╚██████╔╝██║     ██║ ╚═╝ ██║\n");
    printf("╚═╝  ╚═══╝ ╚═════╝   ╚═╝      ╚═╝    ╚═════╝ ╚═╝     ╚═╝     ╚═╝\n");
    printf(C "\n");
    type_effect(G "[BOOT] Initializing modules . . .\n" RS, 8);
    progress_bar("Loading UI assets", 25, 30);
    printf("\n");
}

// ================= Main =================
int main(){
    srand((unsigned)time(NULL));
    splash();
    char choice[16];

    while(true){
        header("MAIN MENU");
        menu();

        if(!fgets(choice, sizeof choice, stdin)) break;
        switch(atoi(choice)){
            case 1: mod_scan(); break;
            case 2: mod_bruteforce(); break;
            case 3: mod_ddos(); break;
            case 4: mod_keylogger(); break;
            case 5: mod_email_breach(); break;
            case 6: mod_exploit_scan(); break;
            case 7: mod_packet_sniffer(); break;
            case 8: mod_ssh_brute(); break;
            case 9: mod_crypto_scan(); break;
            case 10: mod_sql_injection(); break;
            case 11: mod_wifi_cracker(); break;
            case 12: mod_malware_dropper(); break;
            case 13: mod_stegano_scan(); break;
            case 14: mod_ransomware(); break;
            case 15: printf(G "\n[EXIT] " W "Shutting down the program...\n" RS);
                      return 0;
            default: printf(R "[!] " W "Invalid option.\n" RS); sleep_ms(600);
        }
    }
    return 0;
}

