/* **********************************************
* Little game written by Maddy...               *
* -------------------------------               *
* Idea from a computer game Typing of the Dead. *
********************************************** */
#include <iostream.h>
#include <fstream.h>
#include <windows.h>
#include <string.h>
#include <conio.h>


void main(void);
double time_mult(int level, int diff);
int choose_diff(void);
void disp_welcome(void);
void enter_scores(int points);
void disp_scores(void);
void swap_score(int a, int b);
void sort_scores(void);
void read_scores(void);
void write_scores(void);
int read_words(void);
int read_word_file(int filenum, char *filename);


/* Global Shit */
int score_val[5];
string score_name[5];
string word[500][3];
string diffic[3] = { "Easy", "Normal", "Hard" };
int num_of_words[3];
int run = true;
int sound = false;


void main(void)
{
  char input, input2, input3[10], buf[128];
  int progress, diff, points, level, solved, current, freq, practice = false;
  int x, timer, tot_letters, typos, chars_typed, old_word = -1;
  double t_multip, time_typed;
  string disp_word;

  /* Frequency of the screen output, example: freq = 20 means 1000 / 20 = 50Hz */
  freq = 20;

  if (read_words() != 3) {
    cout << "Error reading word-files!";
    return;
  }

  read_scores();
  sort_scores();

  disp_welcome();
  Sleep(3000);

  start:

  points = 0;
  level = 0;
  solved = 0;
  typos = 0;
  chars_typed = 0;
  time_typed = 0;

  diff = choose_diff();

  if (diff == 0) {
    disp_scores();
    write_scores();
    return;
  }
  /* Practice mode */
  else if (diff == 5) {
    practice = true;
    clrscr();
    cout << "Please enter a level (1-3): ";
    cin >> input3;
    level = atoi(input3);
    level--;
    if (level < 0)
      level = 0;
    else if (level > 2)
      level = 2;
    cout << "\nPlease enter a delay in seconds (0.5-30): ";
    cin >> input3;
    t_multip = atof(input3);
    if (t_multip < 0.5)
      t_multip = 0.5;
    else if (t_multip > 30)
      t_multip = 30;
  }

  /* Main Loop? */
  for (; run; ) {
    srand(time(NULL));
    current = rand() % num_of_words[level];
    for (; current == old_word; )
      current = rand() % num_of_words[level];
    old_word = current;
    if (!practice)
      t_multip = time_mult(level, diff);
    timer = t_multip * 1000;
    disp_word = word[current][level];
    tot_letters = disp_word.length();
    for (x = 0; x < tot_letters; x++) {
      if (disp_word[x] == '_')
        disp_word[x] = ' ';
      else
        disp_word[x] = toupper(disp_word[x]);
    }
    progress = 0;

    for (; ; Sleep(freq)) {
      if (level >= 3) {
        run = false;
        break;
      }
      if (progress >= tot_letters) {
        solved++;
        if (!practice) {
          points += tot_letters * diff;
          if (solved >= 10) {
            solved = 0;
            old_word = -1;
            level++;
          }
        }
        time_typed += (t_multip * 1000) - timer;
        break;
      }
      timer -= freq;
      clrscr();
      if (practice)
        cout << "**PRACTICE MODE**\n";
      sprintf(buf, "Diff: [%s], Level: [%d], Solved: [%d%s], Points: [%d]\n", diffic[diff - 1], (level + 1), solved, practice ? "" : "/10", points);
      cout << buf;
      sprintf(buf, "Time: [%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c]\n\n", timer >= (t_multip * 50) ? '#' : '-',
      timer >= (t_multip * 100) ? '#' : '-', timer >= (t_multip * 150) ? '#' : '-',
      timer >= (t_multip * 200) ? '#' : '-', timer >= (t_multip * 250) ? '#' : '-',
      timer >= (t_multip * 300) ? '#' : '-', timer >= (t_multip * 350) ? '#' : '-',
      timer >= (t_multip * 400) ? '#' : '-', timer >= (t_multip * 450) ? '#' : '-',
      timer >= (t_multip * 500) ? '#' : '-', timer >= (t_multip * 550) ? '#' : '-',
      timer >= (t_multip * 600) ? '#' : '-', timer >= (t_multip * 650) ? '#' : '-',
      timer >= (t_multip * 700) ? '#' : '-', timer >= (t_multip * 750) ? '#' : '-',
      timer >= (t_multip * 800) ? '#' : '-', timer >= (t_multip * 850) ? '#' : '-',
      timer >= (t_multip * 900) ? '#' : '-', timer >= (t_multip * 950) ? '#' : '-');
      cout << buf;
      cout << "Word: " << disp_word << "\n";
      cout << "Type: ";
      for (x = 0; x < tot_letters; x++) {
        if (progress >= (1 + x))
          cout << '_';
        else
          cout << disp_word[x];
      }
      if (kbhit()) {
        chars_typed++;
        input = getch();
        input = tolower(input);
        if (input == word[current][level][progress]) {
          progress++;
          if (word[current][level][progress] == '_')
            progress++;

        }
        else {
          points = 0;
          typos++;
        }
      }
      if (timer <= 0) {
        run = false;
        break;
      }
    }
  }
  if (points >= 20 && points > score_val[4]) {
    enter_scores(points);
    write_scores();
  }
  clrscr();
  sprintf(buf, "Points: %d, Typos: %d\n\n", points, typos);
  cout << buf;
  sprintf(buf, "%d characters typed in %.2f seconds.\n", chars_typed, (time_typed / 1000));
  cout << buf;
  if (chars_typed) {
    sprintf(buf, "Average: %.3f seconds per character.\n", ((time_typed / 1000) / chars_typed));
    cout << buf;
  }
  for (;;) {
    cout << "\nPlay again? (y/n)\n";
    input2 = getch();

    if (input2 == 'y') {
	  practice = false;
      run = true;
      goto start;
    }
    else if (input2 == 'n')
      break;
  }
  disp_scores();
}


/* Set the times here, the values are seconds */
double time_mult(int level, int diff)
{
  switch (diff) {
    case 1:
      switch (level) {
        case 0:
          return (5);
        case 1:
          return (7);
        case 2:
          return (15);
        default:
          return (0);
      }
    case 2:
      switch (level) {
        case 0:
          return (1.75);
        case 1:
          return (3.5);
        case 2:
          return (8.75);
        default:
          return (0);
      }
    case 3:
      switch (level) {
        case 0:
          return (1.25);
        case 1:
          return (2.5);
        case 2:
          return (6);
        default:
          return (0);
      }
    default:
      return (0);
  }
}


int choose_diff(void)
{
  char choice;

  for (;;) {
    clrscr();

    cout << "Welcome to Maddy's little word game v1.5!\n";
    cout << "-----------------------------------------\n";
    cout << "\n";
    if (sound)
      cout << "Sound: ON\n";
    else
      cout << "Sound: OFF\n";
    cout << "\n";
    cout << "S = Toggle Sound\n";
    cout << "P = Practice Mode\n";
    cout << "H = Display Highscores\n";
    cout << "Q = Quit\n";
    cout << "\n";
    cout << "Please choose your difficulty:\n";
    cout << "1 = Easy   (for you!)\n";
    cout << "2 = Normal (for advancer typers)\n";
    cout << "3 = Hard   (for typing prodigies)\n";
    cout << "\n";
    cout << " > ";

    choice = getch();

    cout << "\n";

    switch (choice) {
      case '1':
        return 1;
      case '2':
        return 2;
      case '3':
        return 3;
      case 'q':
      case 'Q':
        return 0;
      case 'p':
      case 'P':
        return 5;
      case 's':
      case 'S':
        if (sound)
          sound = false;
        else
          sound = true;
        break;
      case 'h':
      case 'H':
        disp_scores();
        getch();
        break;
      default:
        break;
    }
  }
}


void disp_welcome(void)
{
  clrscr();

  cout << "\n\n";
  cout << "                        ***********\n";
  cout << "                        * CAUTION *\n";
  cout << "                        ***********\n";
  cout << "\n\n";
  cout << "  This program may:\n";
  cout << "\n";
  cout << "     -Cause your CPU to melt. (Due to heavy calculations)\n";
  cout << "     -Suddenly print some weird ascii pr0n to your screen!\n";
  cout << "     -Decrease the size of your penis. (Only applies for Canadians)\n";
}


void enter_scores(int points)
{
  string getname = "1234567890abcdefgh";

  for (; (getname.length() > 16 || getname.length() < 4); ) {
    clrscr();
    cout << "You made the top 5!\n\n";
    cout << "Please enter your name (min 4, max 16 characters).\n";
    cout << " > ";
    cin >> getname;
  }

  score_name[4] = getname;
  score_val[4] = points;

  sort_scores();
}


void disp_scores(void)
{
  char buf[128];
  int x;

  cout << "\n";
  cout << "The Top 5 Typers!\n";
  cout << "~~~~~~~~~~~~~~~~~\n";
  for (x = 1; x <= 5; x++) {
    sprintf(buf, "%d. %5d - ", x, score_val[(x - 1)]);
    cout << buf << score_name[(x - 1)] << "\n";
  }
}


void swap_score(int a, int b)
{
  string temp_name;
  int temp_val;

  temp_name = score_name[a];
  temp_val = score_val[a];

  score_name[a] = score_name[b];
  score_val[a] = score_val[b];

  score_name[b] = temp_name;
  score_val[b] = temp_val;
}


void sort_scores(void)
{
  int x, z;

  for (x = 0; x < 5; x++)
    for (z = 0; z < 5; z++)
      if (x < z && score_val[x] < score_val[z])
        swap_score(z, x);
}


void read_scores(void)
{
  char temp[32], line[32];
  string temp_name;
  int x, temp_val, check;

  ifstream scorefile("scores.txt");

  if (!scorefile.is_open()) {
    for (x = 0; x < 5; x++) {
      score_name[x] = "Nobody";
      score_val[x] = 0;
    }
    return;
  }

  for (x = 0; x < 5; x++) {
    scorefile.getline(line, 32);
    if (sscanf(line, "%s %d %d", temp, &temp_val, &check) == 3) {
      temp_name = string(temp);
      if (temp_val >= 300)
        check *= 2;
      if ((check == (temp_name[temp_name.length() - 1] + temp_name[temp_name.length() - 3]) * temp_val) + (x * 7)) {
        score_name[x] = temp_name;
        score_val[x] = temp_val;
      }
      else {
        score_name[x] = "Nobody";
        score_val[x] = 0;
      }
    }
    else {
      score_name[x] = "Nobody";
      score_val[x] = 0;
    }
  }

  scorefile.close();
}


void write_scores(void)
{
  int x, check;

  ofstream scorefile("scores.txt", ios::trunc);

  for (x = 0; x < 5; x++) {
    if (score_val[x] == 0)
      check = 0;
    else
      check = ((score_name[x][score_name[x].length() - 1] + score_name[x][score_name[x].length() - 3]) * score_val[x]) + (x * 7);
    if (score_val[x] >= 300)
      check /= 2;
    scorefile << score_name[x] << ' ' << score_val[x] << ' ' << check << "\n";
  }

  scorefile.close();
}


int read_words(void)
{
  char *name1 = "word_1.txt";
  char *name2 = "word_2.txt";
  char *name3 = "word_3.txt";
  int retval = 0;

  if (read_word_file(0, name1))
    retval++;
  if (read_word_file(1, name2))
    retval++;
  if (read_word_file(2, name3))
    retval++;

  return (retval);
}


int read_word_file(int filenum, char *filename)
{
  int x, l, c;
  char line[70];

  num_of_words[filenum] = 0;

  ifstream wordfile(filename);

  if (!wordfile.is_open())
    return 0;

  for (l = 0; !wordfile.eof(); l++) {
    wordfile.getline(line, 70);
    word[l][filenum] = string(line);
    num_of_words[filenum]++;
  }

  wordfile.close();

  num_of_words[filenum]--;

  return 1;
}
