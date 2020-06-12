//Fechet Stefan
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <time.h>
#include <panel.h>
#include <stdlib.h>
#include <math.h>
#include <sys/select.h>
#include <locale.h>
#define FOREVER 1
#define SELECT_EVENT 1
#define SELECT_NO_EVENT	0


void move_right(int m[5][5]) {
	int i, j, poz = 3, t, cop[7];
	for (i = 0; i < 4; i++) {
		poz = -1;
		for (t = 0; t < 4; t++)
			cop[t] = 0;
		for (j = 3; j >= 0; j--) {
			if (m[i][j] != 0) {
				poz++;
				cop[poz] = m[i][j];
			}
		}
		for(j = 0; j < 4; j++)
			m[i][j] = cop[3 - j];
	}
}

int calc_right(int m[5][5]) {
	int i, j, s = 0;
	for(i = 0; i < 4; i++)
		for(j = 3; j > 0; j--)
			if(m[i][j] == m[i][j - 1]) {
				m[i][j] *= 2 ;	
				m[i][j - 1] = 0;
				s += m[i][j];
				move_right(m);
			}
	return s;
}

void move_left(int m[5][5]) {
	int i, j, poz = 3, t, cop[7];
	for (i = 0; i < 4; i++) {
		poz = 4;
		for (t = 0; t < 4; t++)
			cop[t] = 0;
		for (j = 0; j <= 3; j++) {
			if (m[i][j] != 0) {
					poz--;
					cop[poz] = m[i][j];
			}
		}
		for (j = 0; j < 4; j++)
			m[i][j] = cop[3 - j];
	}
}

int calc_left(int m[5][5]) {
	int i, j, s = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j <= 3; j++)
			if (m[i][j] == m[i][j + 1]) {
				m[i][j] *= 2 ;
				s += m[i][j];
				m[i][j + 1] = 0;
				move_left(m);
			}
	return s;
}

void move_up(int m[5][5]) {
	int i, j, poz = 3, t, cop[7];
	for (j = 0; j < 4; j++) {
		poz = -1;
		for (t = 0; t < 4; t++)
			cop[t] = 0;
		for (i = 0; i <= 3; i++) {
			if (m[i][j] != 0) {
					poz++;
					cop[poz] = m[i][j];
				}
			}
		for(i = 0; i < 4; i++)
			m[i][j] = cop[i];
	}
}

int calc_up(int m[5][5]) {
	int i, j, s = 0;
	for (j = 0; j < 4; j++)
		for (i = 0; i < 3; i++)
			if (m[i][j] == m[i + 1][j]) {
				m[i][j] *= 2;
				s += m[i][j];
				m[i + 1][j] = 0;
				move_up(m);
			}
	return s;
}

void move_down(int m[5][5]) {
	int i, j, poz = 3, t, cop[7];
	for (j = 0; j <= 3; j++) {
		poz = -1;
		for (t = 0; t < 4; t++)
			cop[t] = 0;
		for (i = 3; i >= 0; i--) {
				if(m[i][j] != 0){
					poz++;
                    cop[poz] = m[i][j];
				}
		}
		for (i = 3; i >= 0; i--)
			m[i][j] = cop[3 - i];
	}
}

int calc_down(int m[5][5]) {
	int i, j, s = 0;
	for (j = 0; j < 4; j++)
		for ( i = 3; i >= 0; i--)
			if (m[i][j] == m[i - 1][j]) {
					m[i][j] *= 2;
					s += m[i][j];
					m[i - 1][j] = 0;
					move_down(m);
				}
	return s;
}

void generare(int m[5][5]) {
	int random_number, t, j, i;
	srand(time(NULL));
	random_number = rand() % 16;
	t = random_number / 4;
	j = random_number % 4;
	while(m[t][j] != 0)
		for (i = 0; i < 1; i++){
			random_number = rand() % 16;
			t = random_number / 4;
			j = random_number % 4;
		}
	random_number = rand() % 6;
	while(random_number != 2 && random_number != 4)
		random_number = rand() % 6;
	m[t][j] = random_number;
}

int nrcif(int n) {
	int nrcif = 0;
	while(n != 0) {
		n = n / 10;
		nrcif++;
	}
	return nrcif;
}

void afisare(int m[5][5], WINDOW *win[50][50]) {
	int i, j;
	for(i = 0; i <= 3; i++)
		for(j = 0; j <= 3; j++) {
			win[i][j] = newwin(3,6,4*i+1,7*j+1);
			wmove(win[i][j], 0, 0);
			wclrtobot(win[i][j]);
			if (nrcif(m[i][j]) <= 2 && nrcif(m[i][j]) > 0)
				for (int p = 1; p <= 6; p++)
					mvprintw(i * 4 + 2, j * 7 + p, " ");	
			if (nrcif(m[i][j]) > 2 && nrcif(m[i][j]) < 5)
				for (int p = 1; p <= 6; p++)
					mvprintw(i * 4 + 2, j * 7 + p, " ");
			if (nrcif(m[i][j]) == 5)
				for (int p = 1; p <= 6; p++)
					mvprintw(i * 4 + 2, j * 7 + p, " ");
			if(m[i][j] == 0)
				for (int p = 1; p <= 6; p++)
					mvprintw(i * 4 + 2, j * 7 + p, " ");
			if (m[i][j] != 0) {
				if (m[i][j] == 2) {
					wbkgd(win[i][j], COLOR_PAIR(1));
					mvprintw(i * 4 + 2, j * 7 + 3, "%d", m[i][j]);
				}
				if (m[i][j] == 4) {
					wbkgd(win[i][j], COLOR_PAIR(2));
					mvprintw(i * 4 + 2, j * 7 + 3, "%d", m[i][j]);
				}
				if (m[i][j] == 8) {
					wbkgd(win[i][j], COLOR_PAIR(3));
					mvprintw(i * 4 + 2, j * 7 + 3, "%d", m[i][j]);
				}
				if (m[i][j] == 16) {
					wbkgd(win[i][j], COLOR_PAIR(4));
					mvprintw(i * 4 + 2, j * 7 + 3, "%d", m[i][j]);
				}
				if (m[i][j] == 32) {
					wbkgd(win[i][j], COLOR_PAIR(5));
					mvprintw(i * 4 + 2, j * 7 + 3, "%d", m[i][j]);
				}
				if (m[i][j] == 64) {
					wbkgd(win[i][j], COLOR_PAIR(6));
					mvprintw(i * 4 + 2, j * 7 + 3, "%d", m[i][j]);
				}
				if (m[i][j] >= 128) {
					wbkgd(win[i][j], COLOR_PAIR(7));
					mvprintw(i * 4 + 2, j * 7 + 3, "%d", m[i][j]);
				}
			}
			wrefresh(win[i][j]);
		}
}

int test_right(int m[5][5]) {
	int i, j, t, copiet[5][5];
	for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++)
			copiet[i][j] = m[i][j];
    move_right(m);
	t = calc_right(m);
	for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++)
            if(copiet[i][j] != m[i][j]) {
				for (i = 0; i <= 3; i++)
					for (t = 0; t <= 3; t++)
						m[i][t] = copiet[i][t];	
				return 1;
			}
	for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++)
			m[i][j]=copiet[i][j];
	return 0;
}

int test_left(int m[5][5]) {
	int i, j, t, copiet[5][5];
	for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++)
			copiet[i][j]=m[i][j];
    move_left(m);
	t = calc_left(m);
	for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++)
            if(copiet[i][j] != m[i][j]) {
				for (i = 0; i <= 3; i++)
					for (t = 0; t <= 3; t++)
						m[i][t] = copiet[i][t];	
				return 1;
			}
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			m[i][j] = copiet[i][j];
	return 0;
}

int test_down(int m[5][5]) {
	int i, j, t, copiet[5][5];
	for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++)
			copiet[i][j]=m[i][j];
    move_down(m);
	t = calc_down(m);
    for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
            if(copiet[i][j] != m[i][j]) {
				for (i = 0; i <= 3; i++)
					for (t = 0; t <= 3; t++)
						m[i][t] = copiet[i][t];	
				return 1;
			}
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			m[i][j] = copiet[i][j];
	return 0;
}

int test_up(int m[5][5]) {
	int i, j, t, copiet[5][5];
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			copiet[i][j] = m[i][j];
    move_up(m);
	t = calc_up(m);
    for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++)
            if(copiet[i][j] != m[i][j]) {
				for (i = 0; i <= 3; i++)
					for (t = 0; t <= 3; t++)
						m[i][t] = copiet[i][t];	
				return 1;
			}
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			m[i][j] = copiet[i][j];
	return 0;
}

int elib_right(int m[5][5]) {
	int i, j, copiet[5][5], nc = 1, nm = 0;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++) {
			copiet[i][j] = m[i][j];
			if (m[i][j] != 0)
				nm++;
		}
    move_right(m);
	calc_right(m);
    for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
            if(m[i][j] !=0)
				nc++;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			m[i][j] = copiet[i][j];
	return nm - nc;
}

int elib_left(int m[5][5]) {
	int i, j, copiet[5][5], nc = 1, nm = 0;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++) {
			copiet[i][j] = m[i][j];
			if (m[i][j] != 0)
				nm++;
		}
    move_left(m);
	calc_left(m);
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
    		if(m[i][j] != 0)
				nc++;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			m[i][j] = copiet[i][j];
	return nm - nc;
}

int elib_down(int m[5][5]) {
	int i, j, copiet[5][5], nc = 1, nm = 0;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++) {
			copiet[i][j] = m[i][j];
			if (m[i][j] != 0)
				nm++;
		}
    move_down(m);
	calc_down(m);
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
    		if(m[i][j] != 0)
				nc++;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			m[i][j] = copiet[i][j];
	return nm - nc;
}

int elib_up(int m[5][5]) {
	int i, j, copiet[5][5], nc = 1, nm = 0;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++) {
			copiet[i][j] = m[i][j];
			if (m[i][j] != 0)
				nm++;
		}
    move_up(m);
	calc_up(m);
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
    		if(m[i][j] != 0)
				nc++;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			m[i][j] = copiet[i][j];
	return nm - nc;
}

int avantaj(int m[5][5]) {
	int aux[6], poz[6], i, j, a, perm[6];
	aux[0] = elib_right(m);
	poz[0] = 0;
	perm[0] = test_right(m);
	aux[1] = elib_left(m);
	poz[1] = 1;
	perm[1] = test_left(m);
	aux[2] = elib_down(m);
	poz[2] = 2;
	perm[2] = test_down(m);
	aux[3] = elib_up(m);
	poz[3] = 3;
	perm[3] = test_up(m);
	for (i = 0; i <= 2; i++)
		for (j = i + 1; j <= 3; j++)
			if (aux[i] > aux[j]) {
				a = aux[i];
				aux[i] = aux[j];
				aux[j] = a;
				a = poz[i];
				poz[i] = poz[j];
				poz[j] = a;
				a = perm[i];
				perm[i] = perm[j];
				perm[j] = a;
			}
	if (aux[3] == -1)
		for (i = 3; i >= 0; i--)
			if (perm[i] != 0)
				return poz[i];
	return poz[3];
}

void copiere(int m[5][5], int copie[5][5]) {
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 3; j++)
			copie[i][j] = m[i][j];
}

void copiere_resume(int m[5][5], int copie_resume[6][6]){
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 3; j++)
			copie_resume[i][j] = m[i][j];
}

void undo(int m[5][5], int copie[5][5]){
	for (int i = 0; i <= 3; i++)
		for (int j = 0; j <= 3; j++)
			m[i][j] = copie[i][j];
}

void afisare_cadru(){
	int i, j;
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 29; j++){
			if (i % 4 == 0)
				mvaddstr(i, j, "-");
			if (i % 4 != 0){
				if (j % 7 == 0)
					mvaddstr(i, j, "|");
				if (j % 7 != 0)
					mvaddstr(i, j, " ");
			}
		}       
	}
	mvaddstr(19, 0, "U = Undo");
	mvaddstr(20, 0, "I = How To ?");
	mvaddstr(21, 0, "T = Tips");
	mvaddstr(22, 0, "Q = Quit");
}

int verificare_nr_blocuri(int copie_resume[6][6]) {
	int i, j, nr = 0;
	for (i = 0; i <= 3; i++)
		for (j = 0; j <=3; j++)
			if (copie_resume[i][j] != 0)
				nr++;
	return nr;
}

int main(void) {
	int i, j, m[5][5], copie[5][5], copie_resume[6][6], scor = 0  ;
	initscr();
	start_color();
	WINDOW *win[50][50];
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    init_pair(4, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(5, COLOR_BLACK, COLOR_RED);
    init_pair(6, COLOR_BLACK, COLOR_YELLOW);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
	for(i = 0; i <= 3; i++)
		for(j = 0; j <= 3; j++)
			m[i][j] = 0;
	for(i = 0; i <= 3; i++)
		for(j = 0; j <= 3; j++)
			copie[i][j] = 0;
	for(i = 0; i <= 4; i++)
		for(j = 0; j <= 4; j++)
			copie_resume[i][j] = 0;
	goto meniu;

	resume: {
		char c, b, gm[50][50];
		initscr();
		clear();
		noecho();
		cbreak();
		curs_set(0);
		refresh();
		int i, j, ok = 0, semne = 1, nou = 0, random_number, nfds = 1, sel, verificare = 1, count = 0;
		for (i = 0; i <= 3; i++)
			for (j = 0; j <= 3; j++)
				m[i][j] = copie_resume[i][j];
		afisare_cadru();
		refresh();
		mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
		mvprintw(4, 47, "        ");
		mvprintw(4, 40, "Score: %d", scor);
		refresh();
		srand(time(NULL));
		afisare(m, win);
		refresh();
		copiere_resume(m, copie_resume);
		scor = copie_resume[0][4];
		mvprintw(6, 40, "Valid moves:");
		if (test_left(m) != 0)	
			mvprintw(8, 55, "A");
		else
			mvprintw(8, 55, " ");
		if (test_right(m) != 0)	
			mvprintw(8, 63, "D");
		else
			mvprintw(8, 63, " ");
		if (test_up(m) != 0)	
			mvprintw(6, 59, "W");
		else
			mvprintw(6, 59, " ");
		if (test_down(m) != 0)	
			mvprintw(8, 59, "S");
		else
			mvprintw(8, 59, " ");
		refresh();
		fd_set read_descriptors;
		struct timeval timeout;
		time_t current_time = time(NULL);
		struct tm *timpp = localtime(&current_time);
		FD_ZERO(&read_descriptors);
		FD_SET(0, &read_descriptors);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		refresh();
		strcpy(gm[0], "Are you sad?");
		strcpy(gm[1], "I told you not to play");
		strcpy(gm[2], "One more time?");
		strcpy(gm[3], "You lost really bad");
		while(1) {
			if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0) {
				clear();
				refresh();
				copie_resume[4][2] = copie_resume[0][4];
				mvprintw(12, 35, "GAME OVER!");	
				count = 11;
				random_number = rand() % 4;
				while (random_number > 3)
					random_number = rand() % 4;
				mvprintw(15, 35, "%s", gm[random_number]);
				mvprintw(17, 35, "You have a great score of: %d", scor);
				if (copie_resume[3][4] < scor) {
					copie_resume[3][4] = scor;
					semne = 0;
				}
				if (semne == 0) {
					mvprintw(18, 35, "You succeeded to get: %d", scor);
					refresh();
				}
				refresh();
				ok = 1;
			}
			if (verificare == 0)
				if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0) {
					c = getchar();
					refresh();
					verificare = 1;
					if (tolower(c) == 'u') {
						if (verificare_nr_blocuri(copie_resume) > 2 || scor > 0) {
							undo(m, copie);
							copiere(m, copie);
							clear();
							afisare(m, win);
							afisare_cadru();
							scor = scor - nou;
							copie_resume[0][4] = scor;
							nou = 0;
							refresh();
							mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
							mvprintw(2, 40, "Date and time: %s",  asctime(timpp));
							mvprintw(4, 47, "        ");
							mvprintw(4, 40, "Score: %d", scor);
							refresh();
							afisare(m, win);
							refresh();
							if (ok == 1){
								clear();
								refresh();
								afisare_cadru();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(2, 40, "Date and time: %s",  asctime(timpp));
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);	
								afisare(m, win);
								refresh();
							}
							refresh();
							break;	
						}	
					}
					if (tolower(c) == 'q'){
						clear();
						refresh();
						goto meniu;
						break;								
					}
				}
			while (verificare == 1) {
				mvprintw(6, 40, "Valid moves:");
				if (test_left(m) != 0)	
					mvprintw(8, 55, "A");
				else
					mvprintw(8, 55, " ");
				if (test_right(m) != 0)	
					mvprintw(8, 63, "D");
				else
					mvprintw(8, 63, " ");
				if (test_up(m) != 0)	
					mvprintw(6, 59, "W");
				else
					mvprintw(6, 59, " ");
				if (test_down(m) != 0)	
					mvprintw(8, 59, "S");
				else
					mvprintw(8, 59, " ");
				refresh();
				sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
				count ++;		
				refresh();
				if (sel == 1){
					c = getchar();
					refresh();
					count = 0;
					if (tolower(c) == 'q') {
						clear();
						refresh();
						goto meniu;
					}
					switch (tolower(c)) {
						case 'a':
							if (test_left(m) != 0) {	
								copiere(m, copie);
								move_left(m);
								nou = calc_left(m);
								scor = scor + nou;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								refresh();
								generare(m);
								refresh();
								afisare(m, win);
								refresh();
								copiere_resume(m, copie_resume);
								copie_resume[0][4] = scor;
								break;
							}
							else
								break;
						case 'd':
							if (test_right(m) != 0) {
								copiere(m, copie);
								move_right(m);
								nou = calc_right(m);
								scor = scor + nou;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								generare(m);
								refresh();
								afisare(m,win);
								refresh();
								copiere_resume(m, copie_resume);
								copie_resume[0][4] = scor;
								break;
							}
							else
								break;
						case 'w':
							if (test_up(m) != 0) {
								copiere(m, copie);
								move_up(m);
								nou = calc_up(m);
								scor = scor + nou;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								refresh();
								generare(m);
								refresh();
								afisare(m, win);
								refresh();
								copiere_resume(m, copie_resume);
								copie_resume[0][4] = scor;
								break;
							}
							else
								break;
						case 's':
							if (test_down(m) != 0) {
								copiere(m, copie);
								move_down(m);
								nou = calc_down(m);
								scor = scor + nou;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								refresh();
								generare(m);
								refresh();
								afisare(m, win);
								refresh();
								copiere_resume(m, copie_resume);
								copie_resume[0][4] = scor;
								break;
							}
							else 
								break;
						case 'u':
							if (verificare_nr_blocuri(copie_resume) > 2 || scor > 0) {
								undo(m, copie);
								copiere(m, copie);
								scor = scor - nou;
								copie_resume[0][4] = scor;
								nou = 0;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								refresh();
								afisare(m, win);
								refresh();
								if (ok == 1) {
									clear();
									refresh();
									afisare_cadru();
									mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
									mvprintw(4, 47, "        ");
									mvprintw(4, 40, "Score: %d", scor);
									afisare(m, win);
									refresh();
								}
								refresh();
								break;	
							}
							else
								break;
						case 'i': {
							clear();
							refresh();
							goto how_to;
							break;
						}
						case 't':{
							clear();
							refresh();
							goto tips;
							break;
						}		
					}
					int s = 0, l = 0, semn = 1;
					for (s = 0; s <= 3; s++)
						for (l = 0; l <= 3; l++)
							if (m[s][l] == 2048 && copie_resume[4][0] == 0) {
								clear();
								refresh();
								mvprintw(12, 35, "You won!");
								copie_resume[4][0] = 1;
								copie_resume[4][3]++;
								mvprintw(20, 6, "Press r/R to continue playing or q/Q to quit");
								refresh();
								while (semn == 1) {
									b = getchar();
									refresh();
									switch (tolower(b)) {
										case 'q': {
											clear();
											refresh();
											goto meniu;
											break;
										}
										case 'r': {
											clear();
											refresh();
											semn = 0;
											mvprintw(6, 40, "Valid moves:");
											if (test_left(m) != 0)	
												mvprintw(8, 55, "A");
											else
												mvprintw(8, 55, " ");
											if (test_right(m) != 0)	
												mvprintw(8, 63, "D");
											else
												mvprintw(8, 63, " ");
											if (test_up(m) != 0)	
												mvprintw(6, 59, "W");
											else
												mvprintw(6, 59, " ");
											if (test_down(m) != 0)	
												mvprintw(8, 59, "S");
											else
												mvprintw(8, 59, " ");
											refresh();
											afisare(m, win);
											refresh();
											afisare_cadru();
											refresh();
											break;	
										}
									}
								}
							}
					refresh();
					afisare(m, win);
					mvprintw(6, 40, "Valid moves:");
					if (test_left(m) != 0)	
						mvprintw(8, 55, "A");
					else
						mvprintw(8, 55, " ");
					if (test_right(m) != 0)	
						mvprintw(8, 63, "D");
					else
						mvprintw(8, 63, " ");
					if (test_up(m) != 0)	
						mvprintw(6, 59, "W");
					else
						mvprintw(6, 59, " ");
					if (test_down(m) != 0)	
						mvprintw(8, 59, "S");
					else
						mvprintw(8, 59, " ");
					refresh();
					mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
					mvprintw(4, 47, "        ");
					mvprintw(4, 40, "Score: %d", scor);
					refresh();			
					if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0) {
						clear();
						refresh();
						copie_resume[4][2] = copie_resume[0][4];
						mvprintw(12, 35, "GAME OVER!");	
						count = 0;
						random_number = rand() % 4;
						while (random_number > 3)
							random_number = rand() % 4;
						mvprintw(15, 35, "%s", gm[random_number]);
						mvprintw(17, 35, "You have a great score of: %d", scor);
						if (copie_resume[3][4] < scor) {
							copie_resume[3][4] = scor;
							semne = 0;
						}
						if (semne == 0) {
							mvprintw(18, 35, "You succeeded to get: %d", scor);
							refresh();
						}
						refresh();
						ok = 1;
					}
				}
				if (sel == 0 && count == 10 && (test_left(m) != 0 || test_right(m) != 0 || test_up(m) != 0 || test_down(m) != 0)) {
					count = 0;
					if (avantaj(m) == 0) {
						copiere(m, copie);
						move_right(m);
						nou = calc_right(m);
						scor = scor + nou;
						refresh();
						mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
						mvprintw(4, 47, "        ");
						mvprintw(4, 40, "Score: %d", scor);
						refresh();
						generare(m);
						refresh();
						afisare(m, win);
						refresh();
						copiere_resume(m, copie_resume);
						copie_resume[0][4] = scor;
						break;
					}
					if (avantaj(m) == 1) {
						copiere(m, copie);
						move_left(m);
						nou = calc_left(m);
						scor = scor + nou;
						refresh();
						mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
						mvprintw(4, 47, "        ");
						mvprintw(4, 40, "Score: %d", scor);		
						generare(m);
						refresh();
						afisare(m, win);
						refresh();
						copiere_resume(m, copie_resume);
						copie_resume[0][4] = scor;
						break;
					}
					if (avantaj(m) == 2) {
						copiere(m, copie);
						move_down(m);
						nou = calc_down(m);
						scor = scor + nou;
						refresh();
						mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
						mvprintw(4, 47, "        ");
						mvprintw(4, 40, "Score: %d", scor);
						refresh();
						generare(m);
						refresh();
						afisare(m, win);
						refresh();
						copiere_resume(m, copie_resume);
						copie_resume[0][4] = scor;
						break;
					}
					if (avantaj(m) == 3) {
						copiere(m, copie);
						move_up(m);
						nou = calc_up(m);
						scor = scor + nou;
						refresh();
						mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
						mvprintw(4, 47, "        ");
						mvprintw(4, 40, "Score: %d", scor);
						refresh();
						generare(m);
						refresh();
						afisare(m, win);
						refresh();
						copiere_resume(m, copie_resume);
						copie_resume[0][4] = scor;
						break;
					}
					int s = 0, l = 0, semn = 1;
					for (s = 0; s <= 3; s++)
						for (l = 0; l <= 3; l++)
							if (m[s][l] == 2048 && copie_resume[4][0] == 0) {
								clear();
								refresh();
								mvprintw(12, 35, "You won!");
								copie_resume[4][0] = 1;
								copie_resume[4][3]++;
								mvprintw(20, 6, "Press r/R to continue playing or q/Q to quit");
								refresh();
								while (semn == 1) {
									b = getchar();
									refresh();
									switch (tolower(b)) {
										case 'q': {
											clear();
											refresh();
											goto meniu;
											break;
										}
										case 'r': {
											clear();
											refresh();
											semn = 0;
											mvprintw(6, 40, "Valid moves:");
											if (test_left(m) != 0)	
												mvprintw(8, 55, "A");
											else
												mvprintw(8, 55, " ");
											if (test_right(m) != 0)	
												mvprintw(8, 63, "D");
											else
												mvprintw(8, 63, " ");
											if (test_up(m) != 0)	
												mvprintw(6, 59, "W");
											else
												mvprintw(6, 59, " ");
											if (test_down(m) != 0)
												mvprintw(8, 59, "S");
											else
												mvprintw(8, 59, " ");
											refresh();
											afisare(m, win);
											refresh();
											afisare_cadru();
											refresh();
											break;	
										}
									}
								}
							}
					refresh();
					afisare(m, win);
					if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0) {
						clear();
						refresh();
						copie_resume[4][2] = copie_resume[0][4];
						mvprintw(12, 35, "GAME OVER!");	
						count = 11;
						random_number = rand() % 4;
						while (random_number > 3)
							random_number = rand() % 4;
						mvprintw(15, 35, "%s", gm[random_number]);
						mvprintw(17, 35, "You have a great score of: %d", scor);
						if (copie_resume[3][4] < scor) {
							copie_resume[3][4] = scor;
							semne = 0;
						}
						if (semne == 0) {
							mvprintw(18, 35, "You succeeded to get: %d", scor);
							refresh();
						}
						refresh();
						ok = 1;
					}
				}
				if (verificare != 0){
					current_time = time(NULL);
					timpp = localtime(&current_time);	
					mvprintw(2, 40, "Date and time: %s",  asctime(timpp));	
					refresh();
					FD_SET(0, &read_descriptors);
					timeout.tv_sec = 1;
					timeout.tv_usec = 0;
				}
			}
		}
		endwin();
	}

	joc: {
		char c, b;
		initscr();
		clear();
		noecho();
		cbreak();
		curs_set(0);
		refresh();
		int nfds, sel;
		fd_set read_descriptors;
		struct timeval timeout;
		time_t current_time = time(NULL);
		struct tm *timpp = localtime(&current_time);
		nfds = 1;
		FD_ZERO(&read_descriptors);
		FD_SET(0, &read_descriptors);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
	  	int count =0;
		refresh();
		char gm[50][50];
		strcpy(gm[0], "Are you sad?");
		strcpy(gm[1], "I told you not to play");
		strcpy(gm[2], "One more time?");
		strcpy(gm[3], "You lost really bad");
		copie_resume[0][4] = 0;
		copie_resume[1][4]++;
		copie_resume[4][0] = 0;
		scor = 0;
		int ok = 0, semne = 1;
		int i, j;
		for (i = 0; i <= 3; i++)
			for (j = 0; j <= 3; j++)
				m[i][j] = 0;
		afisare_cadru();
		refresh();
		int nou = 0;
		mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
		mvprintw(4, 40, "Score: %d", scor);
		refresh();
		int random_number;
		srand(time(NULL));
		generare(m);
		generare(m);
		refresh();
		afisare(m, win);
		refresh();
		copiere_resume(m, copie_resume);
		copie_resume[0][4] = scor;
		int verificare = 1;
		mvprintw(6, 40, "Valid moves:");
		if (test_left(m) != 0)	
			mvprintw(8, 55, "A");
		else
			mvprintw(8, 55, " ");
		if (test_right(m) != 0)	
			mvprintw(8, 63, "D");
		else
			mvprintw(8, 63, " ");
		if (test_up(m) != 0)	
			mvprintw(6, 59, "W");
		else
			mvprintw(6, 59, " ");
		if (test_down(m) != 0)	
			mvprintw(8, 59, "S");
		else
			mvprintw(8, 59, " ");
		refresh();	
		refresh();
		refresh();
		while(1) {
			if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0) {
				clear();
				refresh();
				copie_resume[4][2] = copie_resume[0][4];
				mvprintw(12, 35, "GAME OVER!");	
				count = 11;
				random_number = rand() % 4;
				while (random_number > 3)
					random_number = rand() % 4;
				mvprintw(15, 35, "%s", gm[random_number]);
				mvprintw(17, 35, "You have a great score of: %d", scor);
				if (copie_resume[3][4] < scor){
					copie_resume[3][4] = scor;
					semne = 0;
				}
				if (semne == 0){
					mvprintw(18, 35, "You succeeded to get: %d", scor);
					refresh();
				}
				refresh();
				ok = 1;
			}
			if (verificare == 0)
				if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0) {
					c = getchar();
					refresh();
					verificare = 1;
					if (tolower(c) == 'u'){
						if (verificare_nr_blocuri(copie_resume) > 2 || scor > 0) {
							undo(m, copie);
							copiere(m, copie);
							clear();
							afisare(m, win);
							afisare_cadru();
							scor = scor - nou;
							copie_resume[0][4] = scor;
							nou = 0;
							refresh();
							mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
							mvprintw(2, 40, "Date and time: %s",  asctime(timpp));
							mvprintw(4, 47, "        ");
							mvprintw(4, 40, "Score: %d", scor);	
							refresh();
							afisare(m, win);
							refresh();
							if (ok == 1) {
								clear();
								refresh();
								afisare_cadru();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(2, 40, "Date and time: %s",  asctime(timpp));
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);	
								afisare(m, win);
								refresh();
							}
							refresh();
							break;	
						}		
					}
					if (tolower(c) == 'q'){
						clear();
						refresh();
						goto meniu;
						break;								
					}				
				}
			while (verificare == 1) {
				mvprintw(6, 40, "Valid moves:");
				if (test_left(m) != 0)	
					mvprintw(8, 55, "A");
				else
					mvprintw(8, 55, " ");
				if (test_right(m) != 0)	
					mvprintw(8, 63, "D");
				else
					mvprintw(8, 63, " ");
				if (test_up(m) != 0)	
					mvprintw(6, 59, "W");
				else
					mvprintw(6, 59, " ");
				if (test_down(m) != 0)	
					mvprintw(8, 59, "S");
				else
					mvprintw(8, 59, " ");
				refresh();
		
				sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
				count ++;		
				refresh();
				if (sel == 1) {
					c = getchar();
					refresh();
					count = 0;
					if (tolower(c) == 'q') 
						{
							clear();
							refresh();
							goto meniu;
						}
					switch (tolower(c)) {
						case 'a':
							if (test_left(m) != 0) {	
								copiere(m, copie);
								move_left(m);
								nou = calc_left(m);
								scor = scor + nou;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								refresh();
								generare(m);
								refresh();
								afisare(m, win);
								refresh();
								copiere_resume(m, copie_resume);
								copie_resume[0][4] = scor;
								break;
							}
							else
								break;
						case 'd':
							if (test_right(m) != 0) {
								copiere(m, copie);
								move_right(m);
								nou = calc_right(m);
								scor = scor + nou;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								generare(m);
								refresh();
								afisare(m, win);
								refresh();
								copiere_resume(m, copie_resume);
								copie_resume[0][4] = scor;
								break;
							}
							else
								break;
						case 'w':
							if (test_up(m) != 0){
								copiere(m, copie);
								move_up(m);
								nou = calc_up(m);
								scor = scor + nou;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								refresh();
								generare(m);
								refresh();
								afisare(m, win);
								refresh();
								copiere_resume(m, copie_resume);
								copie_resume[0][4] = scor;
								break;
							}
							else
								break;
						case 's':
							if (test_down(m) != 0) {
								copiere(m, copie);
								move_down(m);
								nou = calc_down(m);
								scor = scor + nou;
								refresh();
								mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
								mvprintw(4, 47, "        ");
								mvprintw(4, 40, "Score: %d", scor);
								refresh();
								generare(m);
								refresh();
								afisare(m, win);
								refresh();
								copiere_resume(m, copie_resume);
								copie_resume[0][4] = scor;
								break;
							}
							else 
								break;
						case 'u': {
							if (verificare_nr_blocuri(copie_resume) > 2 || scor > 0) {
									undo(m, copie);
									copiere(m, copie);
									copiere_resume(m, copie_resume);
									scor = scor - nou;
									copie_resume[0][4] = scor;
									nou = 0;
									refresh();
									mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
									mvprintw(4, 47, "        ");
									mvprintw(4, 40, "Score: %d", scor);
									refresh();
									afisare(m, win);
									refresh();
									if (ok == 1) {
										clear();
										refresh();
										afisare_cadru();
										mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
										mvprintw(4, 47, "        ");
										mvprintw(4, 40, "Score: %d", scor);
										afisare(m, win);
										refresh();
									}
									refresh();
									break;	
								}
							else
								break;
						}
						case 'i': {
							clear();
							refresh();
							goto how_to;
							break;
						}
						case 't': {
							clear();
							refresh();
							goto tips;
							break;
						}	
					}
					int s = 0, l = 0, semn = 1;
					for (s = 0; s <= 3; s++)
						for (l = 0; l <= 3; l++)
							if (m[s][l] == 2048 && copie_resume[4][0] == 0) {
								clear();
								refresh();
								mvprintw(12, 35, "You won!");
								copie_resume[4][0] = 1;
								copie_resume[4][3]++;
								mvprintw(20, 6, "Press r/R to continue playing or q/Q to quit");
								refresh();
								while (semn == 1) {
									b = getchar();
									refresh();
									switch (tolower(b)) {
										case 'q': {
											clear();
											refresh();
											goto meniu;
											break;
										}
										case 'r': {
											clear();
											refresh();
											semn = 0;
											mvprintw(6, 40, "Valid moves:");
											if (test_left(m) != 0)	
												mvprintw(8, 55, "A");
											else
												mvprintw(8, 55, " ");
											if (test_right(m) != 0)	
												mvprintw(8, 63, "D");
											else
												mvprintw(8, 63, " ");
											if (test_up(m) != 0)	
												mvprintw(6, 59, "W");
											else
												mvprintw(6, 59, " ");
											if (test_down(m) != 0)	
												mvprintw(8, 59, "S");
											else
												mvprintw(8, 59, " ");
											refresh();
											afisare(m, win);
											refresh();
											afisare_cadru();
											refresh();
											break;	
										}
									}
								}
							}
					refresh();
					afisare(m, win);
					mvprintw(6, 40, "Valid moves:");
					if (test_left(m) != 0)	
						mvprintw(8, 55, "A");
					else
						mvprintw(8, 55, " ");
					if (test_right(m) != 0)	
						mvprintw(8, 63, "D");
					else
						mvprintw(8, 63, " ");
					if (test_up(m) != 0)	
						mvprintw(6, 59, "W");
					else
						mvprintw(6, 59, " ");
					if (test_down(m) != 0)	
						mvprintw(8, 59, "S");
					else
						mvprintw(8, 59, " ");
					refresh();
					mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
					mvprintw(4, 47, "        ");
					mvprintw(4, 40, "Score: %d", scor);
					refresh();						
					if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0) {
						clear();
						refresh();
						copie_resume[4][2] = copie_resume[0][4];
						mvprintw(12, 35, "GAME OVER!");	
						count = 0;
						random_number = rand() % 4;
						while (random_number > 3)
							random_number = rand() % 4;
						mvprintw(15, 35, "%s", gm[random_number]);
						mvprintw(17, 35, "You have a great score of: %d", scor);
						if (copie_resume[3][4] < scor){
							copie_resume[3][4] = scor;
							semne = 0;
						}
						if (semne == 0){
							mvprintw(18, 35, "You succeeded to get: %d", scor);
							refresh();
						}
						refresh();
						ok = 1;
					}
				}
				if (sel == 0 && count == 10 && (test_left(m) != 0 || test_right(m) != 0 || test_up(m) != 0 || test_down(m) != 0)) {
				count = 0;
				if (avantaj(m) == 0) {
					copiere(m, copie);
					move_right(m);
					nou = calc_right(m);
					scor = scor + nou;
					refresh();
					mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
					mvprintw(4, 47, "        ");
					mvprintw(4, 40, "Score: %d", scor);
					refresh();
					generare(m);
					refresh();
					afisare(m, win);
					refresh();
					copiere_resume(m, copie_resume);
					copie_resume[0][4] = scor;
					break;
				}
				if (avantaj(m) == 1) {
					copiere(m, copie);
					move_left(m);
					nou = calc_left(m);
					scor = scor + nou;
					refresh();
					mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
					mvprintw(4, 47, "        ");
					mvprintw(4, 40, "Score: %d", scor);		
					generare(m);
					refresh();
					afisare(m, win);
					refresh();
					copiere_resume(m, copie_resume);
					copie_resume[0][4] = scor;
					break;
				}
				if (avantaj(m) == 2) {
					copiere(m, copie);
					move_down(m);
					nou = calc_down(m);
					scor = scor + nou;
					refresh();
					mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
					mvprintw(4, 47, "        ");
					mvprintw(4, 40, "Score: %d", scor);
					refresh();
					generare(m);
					refresh();
					afisare(m, win);
					refresh();
					copiere_resume(m, copie_resume);
					copie_resume[0][4] = scor;
					break;
				}
				if (avantaj(m) == 3) {
					copiere(m, copie);
					move_up(m);
					nou = calc_up(m);
					scor = scor + nou;
					refresh();
					mvprintw(3, 40, "Record: %d", copie_resume[3][4]);
					mvprintw(4, 47, "        ");
					mvprintw(4, 40, "Score: %d", scor);
					refresh();
					generare(m);
					refresh();
					afisare(m, win);
					refresh();
					copiere_resume(m, copie_resume);
					copie_resume[0][4] = scor;
					break;
				}
				int s = 0, l = 0, semn = 1;
				for (s = 0; s <= 3; s++)
					for (l = 0; l <= 3; l++)
						if (m[s][l] == 2048 && copie_resume[4][0] == 0) {
							clear();
							refresh();
							mvprintw(12, 35, "You won!");
							copie_resume[4][0] = 1;
							copie_resume[4][3]++;
							mvprintw(20, 6, "Press r/R to continue playing or q/Q to quit");
							refresh();
							while (semn == 1) {
								b = getchar();
								refresh();
								switch (tolower(b)) {
									case 'q': {
										clear();
										refresh();
										goto meniu;
										break;
									}
									case 'r': {
										clear();
										refresh();
										semn = 0;
										mvprintw(6, 40, "Valid moves:");
										if (test_left(m) != 0)	
											mvprintw(8, 55, "A");
										else
											mvprintw(8, 55, " ");
										if (test_right(m) != 0)	
											mvprintw(8, 63, "D");
										else
											mvprintw(8, 63, " ");
										if (test_up(m) != 0)	
											mvprintw(6, 59, "W");
										else
											mvprintw(6, 59, " ");
										if (test_down(m) != 0)	
											mvprintw(8, 59, "S");
										else
											mvprintw(8, 59, " ");
										refresh();
										afisare(m, win);
										refresh();
										afisare_cadru();
										refresh();
										break;	
									}
								}
							}
						}
				refresh();
				afisare(m, win);
				if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0) {
					clear();
					refresh();
					copie_resume[4][2] = copie_resume[0][4];
					mvprintw(12, 35, "GAME OVER!");	
					count = 11;
					random_number = rand() % 4;
					while (random_number > 3)
						random_number = rand() % 4;
					mvprintw(15, 35, "%s", gm[random_number]);
					mvprintw(17, 35, "You have a great score of: %d", scor);
					if (copie_resume[3][4] < scor) {
						copie_resume[3][4] = scor;
						semne = 0;
					}
					if (semne == 0) {
						mvprintw(18, 35, "You succeeded to get: %d", scor);
						refresh();
					}
					refresh();
					ok = 1;
				}	
				}
				if (verificare != 0) {
				current_time = time(NULL);
				timpp = localtime(&current_time);	
				mvprintw(2, 40, "Date and time: %s",  asctime(timpp));	
				refresh();
				FD_SET(0, &read_descriptors);
				timeout.tv_sec = 1;
				timeout.tv_usec = 0;
				}
			}
		}
		endwin();
	}

	how_to: {
		char c;
		initscr();
		clear();
		noecho();
		cbreak();
		curs_set(0);
		refresh();
		mvprintw(4, 35, " INSTRUCTIONS");
		mvprintw(5, 35, "**************");
		mvprintw(9, 35, "-----> MOVE UP");
		mvprintw(10, 35, "|");
		mvprintw(11, 35, "|");
		mvprintw(12, 35, "W");
		mvprintw(14, 5, "        MOVE LEFT <----- A    S    D -----> MOVE RIGHT");
		mvprintw(17, 35, "-----> MOVE DOWN");
		mvprintw(15, 35, "|");
		mvprintw(16, 35, "|");
		mvprintw(19, 5, "WARNING!!! USE ONLY THESE KEYS, OTHERWISE ERRORS MIGHT OCCUR!");
		mvprintw(21, 5, "Press u/U for undo");
		mvprintw(22, 5, "During the game you can see the instructions again by pressing i/I");
		mvprintw(23, 5, "Press q/Q to return to menu");
		refresh();
		while (FOREVER) {
			c = getchar();
			refresh();
			if (tolower(c) == 'q') {
				clear();
				refresh();
				goto meniu;	
			}
		}
	}

	tips: {
		char c;
		initscr();
		clear();
		noecho();
		cbreak();
		curs_set(0);
		refresh();
		mvprintw(4, 35, " USEFUL TIPS");
		mvprintw(5, 35, "*************");
		mvprintw(9, 4, "-----> Try to keep the box with the highest value in a corner");
		mvprintw(11, 4, "-----> Be careful when you make a move! ");
		mvprintw(12, 4, "       Undo brings you just one step backwards");
		mvprintw(14, 4, "-----> You can go one step backwards even if it is GAME OVER");
		mvprintw(15, 4, "       Just press the U key");
		mvprintw(17, 4, "-----> Press undo, even if it is GAME OVER");
		mvprintw(18, 4, "       It is possible that another number to be put in the");
		mvprintw(19, 4, "       empty box and you may continue");
		mvprintw(22, 4, "Press q/Q to return to menu");
		refresh();
		while (FOREVER) {
			c = getchar();
			refresh();
			if (tolower(c) == 'q') {
				clear();
				refresh();
				goto meniu;	
			}
		}
	}

	statistic: {
		char c;
		initscr();
		clear();
		noecho();
		cbreak();
		curs_set(0);
		refresh();
		mvprintw(4, 35, "  STATISTICS");
		mvprintw(5, 35, "**************");
		mvprintw(9, 4, "-----> The number of started games (New Game):  %d", copie_resume[1][4]);
		mvprintw(11, 4, "-----> The number of finished games (GAME OVER): %d", copie_resume[2][4]);
		mvprintw(13, 4, "-----> Record: %d", copie_resume[3][4]);
		mvprintw(15, 4, "-----> The number of won games: %d", copie_resume[4][3]);
		int t;
		if (copie_resume[2][4] != 0) {
			t = copie_resume[4][4] / copie_resume[2][4];
			mvprintw(17, 4, "-----> Average score**: %d", t);
		}
		else
			mvprintw(17, 4, "-----> Average score**: 0");
		mvprintw(19, 4, "** It is calculated dividing the sum of all the scores of the ");
		mvprintw(20, 4, "finished games at the number of the finished games");
		mvprintw(23, 4, "Press q/Q to return to menu");
		refresh();
		while (FOREVER) {
			c = getchar();
			refresh();
			if (tolower(c) == 'q') {
				clear();
				refresh();
				goto meniu;	
			}
		}
	}

	meniu: {
		int c;
		initscr();
		setlocale(LC_ALL, "");
		clear();
		noecho();
		keypad(stdscr, TRUE);
		refresh();
		curs_set(0);
		refresh();
		char ng[60][60];
		strcpy(ng[0], "Are you sure you want to play?");
		strcpy(ng[1], "Do you think you are cool?");
		strcpy(ng[2], "You will lose");
		strcpy(ng[3], "Do you know how to play? If not, watch some tutorials");
		strcpy(ng[4], "You would better play FIFA");
		strcpy(ng[5], "Don't play this game if you don't hava patience");
		strcpy(ng[6], "Do you still want to lose?");
		char re[50][50];
		strcpy(re[0], "Do you want to continue?");
		strcpy(re[1], "You should start again");
		strcpy(re[2], "Come on, start a new game");
		char qt[50][50];
		strcpy(qt[0], "Have you started to cry?");
		strcpy(qt[1], "I told you not to play");
		strcpy(qt[2], "Are you tired?");
		strcpy(qt[3], "Are you leaving?");
		strcpy(qt[4], "Are you bored?");
		strcpy(qt[5], "Please, don't go...");
		int ok = 1, i, j, y = 1, random_number, b;
		srand(time(NULL));		
		clear();
		refresh();
		for (i = 5; i <= 7; i++)
			for(j = 2; j <= 14 ;j++) {
				if (i == 5 || i == 7) {
					mvaddstr(i, j, "*");	
					refresh();
				}
				if (j == 2 || j == 14) {
					mvaddstr(i, j, "*");	
					refresh();
				}
			}
		random_number = rand() % 8;
		while(random_number > 6)
			random_number = rand() % 8;
		mvprintw(6, 4, "New Game");
		mvprintw(6, 17, "%s", ng[random_number]);
		mvprintw(8, 5, "Resume");
		mvprintw(10, 4, "How To ?");
		mvprintw(12, 6, "Tips");
		mvprintw(14, 4, "Statistic");	
		mvprintw(16, 6, "Quit");
		refresh();
		while (FOREVER) {
			c = getch();
			refresh();
			y = 1;
			if (c == '\n') {
				if (ok == 1) {
					if (test_left(m) == 0 && test_right(m) == 0 && test_up(m) == 0 && test_down(m) == 0 && verificare_nr_blocuri(copie_resume) >= 2) {
						copie_resume[2][4]++;
						copie_resume[4][4] = copie_resume[4][4] + copie_resume[4][2];
					}
					clear();
					goto joc;
				}
				if (ok == 2 && verificare_nr_blocuri(copie_resume) >= 2) {		
					clear();
					goto resume;
				}
				if (ok == 3){
					clear();
					goto how_to;
				}
				if (ok == 4) {
					clear();
					goto tips;
				}
				if (ok == 5) {
					clear();
					goto statistic;
				}
				if (ok == 6) {
					ok = 7;
					mvprintw(16, 17, "                                         ");
					mvprintw(16, 17, "No");
					mvprintw(18, 16, "Yes");
					refresh();
					for (i = 15; i <= 17; i++)
						for(j = 14; j <= 20 ;j++) {
							if (i == 15 || i == 17) {
								mvaddstr(i, j, "*");	
								refresh();
							}
							if (j == 14 || j == 20) {
								mvaddstr(i, j, "*");	
								refresh();
							}
						}
					while (y == 1) {
						b = getch();
						refresh();
						switch (b) {
							case KEY_DOWN: {
								ok ++;
								if (ok == 9)
									ok = 7;
								break;
							}
							case KEY_UP: {
								ok --;
								if (ok == 6)
									ok = 8;
								break;
							}
						}
						if (ok == 7) {
							for (i = 15; i <= 17; i++)
								for(j = 14; j <= 20 ;j++) {
									if (i == 15) {
										mvaddstr(i, j, "*");	
										refresh();
									}
									if (j == 20) {
										mvaddstr(i, j, "*");	
										refresh();
									}
								}
							for (i = 18; i <= 19; i++)
								for(j = 14; j <= 20 ;j++) {
									if (i == 19) {
										mvaddstr(i, j, " ");	
										refresh();
									}
									if (j == 20 || j == 14) {
										mvaddstr(i, j, " ");	
										refresh();
									}
								}
						}
						if (ok == 8) {
							for (i = 15; i <= 17; i++)
								for(j = 14; j <= 20; j++) {
									if (i == 15) {
										mvaddstr(i, j, " ");	
										refresh();
										if (j == 14)
											mvaddstr(i, j, "*");
									}
									if (j == 20) {
										mvaddstr(i, j, " ");	
										refresh();
									}
								}
								for (i = 17; i <= 19; i++)
									for(j = 14; j <= 20 ;j++) {
											if (i == 19) {
												mvaddstr(i, j, "*");	
												refresh();
											}
											if (j == 20 || j == 14) {
												mvaddstr(i, j, "*");	
												refresh();
											}
										}
						}
						if (b == '\n') {
							if (ok == 8)
								system("pkill 2048");
							else {
								for (i = 15; i <= 19; i++)
									for(j = 14; j <= 20; j++) {
										if (i == 15) {
											mvaddstr(i, j, " ");	
											refresh();
										}
										if (j == 20) {
											mvaddstr(i, j, " ");	
											refresh();
										}
									}
								ok = 1;
								y = 0;
							}
						}
					}
				}
			}
			switch (c) {
				case KEY_DOWN: {
					ok ++;
					if (ok == 7)
						ok = 1;
					break;
				}
				case KEY_UP: {
					ok --;
					if (ok == 0)
						ok = 6;
					break;
				}	
			}
			if (ok == 1){
				clear();
				refresh();
				for (i = 5; i <= 7; i++)
					for(j = 2; j <= 14 ; j++) {
						if (i == 5 || i == 7) {
							mvaddstr(i, j, "*");	
							refresh();
						}
						if (j == 2 || j == 14) {
							mvaddstr(i, j, "*");	
							refresh();
						}
					}
				random_number = rand() % 8;
				while(random_number > 6)
					random_number = rand() % 8;
				mvprintw(6, 4, "New Game");
				mvprintw(6, 17, "%s", ng[random_number]);
				mvprintw(8, 5, "Resume");
				mvprintw(10, 4, "How To ?");
				mvprintw(12, 6, "Tips");
				mvprintw(14, 4, "Statistic");	
				mvprintw(16, 6, "Quit");
				refresh();	
			}
			if (ok == 2) {
				clear();
				refresh();
				for (i = 7; i <= 9; i++)
					for(j = 2; j <= 14 ; j++) {
						if (i == 7 || i == 9) {
							mvaddstr(i, j, "*");	
							refresh();
						}
						if (j == 2 || j == 14) {
							mvaddstr(i, j, "*");	
							refresh();
						}
					}
				mvprintw(6, 4, "New Game");
				mvprintw(8, 5, "Resume");
				mvprintw(10, 4, "How To ?");
				mvprintw(12, 6, "Tips");
				mvprintw(14, 4, "Statistic");	
				mvprintw(16, 6, "Quit");
				refresh();
				if (verificare_nr_blocuri(copie_resume) < 2) {
					mvprintw(8, 17, "First of all, start a new game");
					refresh();
				}
				if (verificare_nr_blocuri(copie_resume) >= 2) {
					random_number = rand() % 4;
					while(random_number > 2)
						random_number = rand() % 4;
					mvprintw(8, 17, "%s", re[random_number]);
					refresh();
				}
			}
			if (ok == 3) {
				clear();
				refresh();
				for (i = 9; i <= 11; i ++)
					for(j = 2; j <= 14;j++) {
						if (i == 9 || i == 11) {
							mvaddstr(i, j, "*");	
							refresh();
						}
						if (j == 2 || j == 14) {
							mvaddstr(i, j, "*");	
							refresh();
						}
					}
				mvprintw(6, 4, "New Game");
				mvprintw(10, 17, "Keys you press");
				mvprintw(8, 5, "Resume");
				mvprintw(10, 4, "How To ?");
				mvprintw(12, 6, "Tips");
				mvprintw(14, 4, "Statistic");	
				mvprintw(16, 6, "Quit");
				refresh();	
			}
			if (ok == 4) {
				clear();
				refresh();
				for (i = 11; i <= 13; i++)
					for(j = 2; j <= 14; j++) {
						if (i == 11 || i == 13) {
							mvaddstr(i, j, "*");	
							refresh();
						}
						if (j == 2 || j == 14) {
							mvaddstr(i, j, "*");	
							refresh();
						}
					}
				mvprintw(6, 4, "New Game");
				mvprintw(12, 17, "Tips for beginners");
				mvprintw(8, 5, "Resume");
				mvprintw(10, 4, "How To ?");
				mvprintw(12, 6, "Tips");
				mvprintw(14, 4, "Statistic");	
				mvprintw(16, 6, "Quit");
				refresh();	
			}
			if (ok == 5) {
				clear();
				refresh();
				for (i = 13; i <= 15; i++)
					for(j = 2; j <= 14; j++) {
						if (i == 13 || i == 15) {
							mvaddstr(i, j, "*");	
							refresh();
						}
						if (j == 2 || j == 14) {
							mvaddstr(i, j, "*");	
							refresh();
						}
					}
				mvprintw(6, 4, "New Game");
				mvprintw(14, 17, "Statistics. If you are interested");
				mvprintw(8, 5, "Resume");
				mvprintw(10, 4, "How To ?");
				mvprintw(12, 6, "Tips");
				mvprintw(14, 4, "Statistic");	
				mvprintw(16, 6, "Quit");
				refresh();	
			}
			if (ok == 6) {
				clear();
				refresh();
				for (i = 15; i <= 17; i++)
					for(j = 2; j <= 14; j++) {
						if (i == 15 || i == 17) {
							mvaddstr(i, j, "*");	
							refresh();
						}
						if (j == 2 || j == 14) {
							mvaddstr(i, j, "*");	
							refresh();
						}
					}
				random_number = rand() % 7;
				while(random_number > 5)
					random_number = rand() % 7;
				mvprintw(6, 4, "New Game");
				mvprintw(8, 5, "Resume");
				mvprintw(10, 4, "How To ?");
				mvprintw(12, 6, "Tips");
				mvprintw(14, 4, "Statistic");	
				mvprintw(16, 6, "Quit");
				refresh();
				mvprintw(16, 17, "%s", qt[random_number]);
				refresh();		
			}
		}
	}
	return 0;
}