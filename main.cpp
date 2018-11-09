#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cmath>
#include <Windows.h>
#include "include/view.hpp"
#include "include/model.hpp"
#include "include/console.hpp"

#define TIME_DELAY   130    // thoi gian dung

static HANDLE handle;

void ViewInitMainWindow();
int Menu();
void MenuSearhDict(Word *, int);
void MenuEditDatabase(Word *, int &);
void Introduce();
int ExitProgram();

/* CHUONG TRINH CHINH */
int main()
{
    // luu du lieu - do dai mang
    Word *arrWord = new Word[MAX_ARR];
    int length = 0; 
    int choose;

    // Khoi tao man hinh console
    ViewInitMainWindow();

    // doc - sap xep - ghi data lan dau
    ReadData(arrWord, length);
    Sort(arrWord, length);
    SaveData(arrWord, length);
    ReadData(arrWord, length);
    
    //LoadingBar();

	int running = 1;
    do
    {
    	choose = Menu();
        switch(choose)
        {
            // tra tu
            case 0:
                MenuSearhDict(arrWord, length);
                break;
            // chinh sua database
            case 1:
                MenuEditDatabase(arrWord, length);
                break;
            //gioi thieu
            case 2:
                Introduce();
                break;
            //thoat ung dung
            case 3:
            	if (ExitProgram() == 1)
            		running = 0;
            	break;
        }

        fflush(stdin);
    }while (running != 0);

    return 0;
}


/* CAI DAT MAN HINH CONSOLE */
void ViewInitMainWindow()
{
	system("MODE CON: COLS=114 LINES=37");
	system("COLOR 0F");
	system("MODE CON: CP SELECT=65001");
	system("TITLE TU DIEN ANH VIET - NHOM 13");
    SetFont(18);

    clrscr();
}


/* MENU */
int Menu()
{
    clrscr();  // xoa man hinh

    int choose = 0;
    const int x1 = 37, x2 = 77;
    int y = 12;

    // tieu de cho cac chuc nang
    char *arrTitle[] = {"TRA TỪ", "CHỈNH SỬA DATABASE", "GIỚI THIỆU", "THOÁT"};

    // ve logo
    DrawLogo(10, 4);

    // xuat ra cac nut chuc nang
    for (int i = 0; i < 4; i++)
        Button(x1, x2, y + (i * 4) + 1, arrTitle[i], POS_CENTER, PRIMARY_COLOR);

    // hien thi button duoc chon
    Button(x1, x2, y + (choose * 4 + 1), arrTitle[choose], POS_CENTER, SECONDARY_COLOR);
    
    int running = 0;    // chuong trinh co dang chay khong
    KEY KeyPressed = KEY_NONE; // bat su kien phim
    while (!running)
    {
        // lay phim nhan
        KeyPressed = GetInputKey();

        switch (KeyPressed)
        {
            // PHIM LEN
            case KEY_UP:
                if (choose > 0)
                {
                    Button(x1, x2, y + (choose * 4 + 1), arrTitle[choose], POS_CENTER, PRIMARY_COLOR);
                    choose--;
                    Button(x1, x2, y + (choose * 4 + 1), arrTitle[choose], POS_CENTER, SECONDARY_COLOR);
                }
                break;

            // PHIM XUONG
            case KEY_DOWN:
                if (choose < 3)
                {
                    Button(x1, x2, y + (choose * 4 + 1), arrTitle[choose], POS_CENTER, PRIMARY_COLOR);
                    choose++;
                    Button(x1, x2, y + (choose * 4 + 1), arrTitle[choose], POS_CENTER, SECONDARY_COLOR);
                }
                break;

            // PHIM ENTER
            case KEY_ENTER:
                running = 1;
                break;

            // KHONG NHAN PHIM NAO
            case KEY_NONE:
                break;
        }

        Sleep(TIME_DELAY);
    }

    TextColor(default_ColorCode);

    return choose;
}


/* CHUC NANG THOAT UNG DUNG */
int ExitProgram()
{
	int choose = ShowMessageBox("Ban co muon thoat chuong trinh khong ?", "Canh bao", 1);
	
	if (choose != 1)
	{
		getchar();
		Sleep(50);
	}
		
	return choose;
}


/* XUAT RA DANH SACH TU (START - END) */
void OutputListDictKey(Word *arrWord, int start, int end)
{
    //set text color
    TextColor(default_ColorCode);
    int x = 2, y = 5, count = 0;

    // xuat tung tu
    for (int i = 0; i < (end - start); i++)
    {
        gotoXY(x, y++);
        puts(arrWord[i + start].key);
    }
}


/* XUAT RA TU LOAI - NGHIA CUA TU */
void OutputDescription(Word word)
{
    int x = 45;
    int y = 5;

    // lam sach nghia truoc do
	TextColor(default_ColorCode);
	for (int j = 2; j < 21; j++)
	{
		for (int i = x; i < 113; i++)
		{
			gotoXY(i, j);
			putch(' ');
		}
	}

    // ve khung
    DrawFrame(43, 113, 0, 20, PRIMARY_COLOR);
    gotoXY(72, 0);
    puts("NGHĨA CỦA TỪ");

    // set text color default
    TextColor(default_ColorCode);

    // in tu loai
    gotoXY(x, 2);
    printf("(+) TỪ LOẠI: %s", word.type);

    //in nghia
    gotoXY(x, 4);
    puts("(+) NGHĨA");
    gotoXY(x, y);
    
    // xuất nghĩa của từ
    char meaning[500];
    strcpy(meaning, word.meaning);

    char *descr = strtok(meaning, "-");
    while (descr != NULL)
    {
        if (strcmp(descr, " ") != 0)
        {
            gotoXY(x, y++);
            printf("- %s\n", descr);
        }
        descr = strtok(NULL, "-");
    }
}


/* HIEN KHUNG TU DUOC CHON DEN */
void WordSelected(int x, int y, Word word, int isSelect)
{
    // neu tu duoc chon thi co mau nen
    TextColor(isSelect ? PRIMARY_COLOR : default_ColorCode);
    gotoXY(x, y);

    // xuat tu duoc chon
    char ch;
    for (int i = 0; i < 37; i++)
    {
        ch = i < strlen(word.key) - 1 ? word.key[i] : ' ';
        putch(ch);
    }

    // neu tu duoc chon thi se hien thi nghia
    if (isSelect) OutputDescription(word);
}


/* XUAT RA HDSD MENU TRA TU */
void UserManualMenuSearch()
{
    gotoXY(70, 21);
    printf("HƯỚNG DẪN SỬ DỤNG");
    gotoXY(64, 22);
    printf("(Nhấn để thực hiện chức năng)");

    Button(55, 77, 23, "ESC. THOÁT", POS_LEFT, PRIMARY_COLOR);
    Button(80, 102, 23, "F1. TÌM KIẾM", POS_LEFT, PRIMARY_COLOR);
    Button(55, 77, 27, "↑. DI CHUYỂN", POS_LEFT, PRIMARY_COLOR);
    Button(80, 102, 27, "↓. DI CHUYỂN XUỐNG", POS_LEFT, PRIMARY_COLOR);
}


/* XUAT RA HDSD MENU CHINH SUA DATABASE*/
void UserManualMenuEdit()
{
    // thoat
    Button(44, 65, 22, "ESC. THOÁT", POS_LEFT, PRIMARY_COLOR);

    // di chuyen
    Button(68, 89, 22, "↑↓. DI CHUYỂN", POS_LEFT, PRIMARY_COLOR);

    // tim kiem
    Button(92, 113, 22, "F1. TÌM KIẾM", POS_LEFT, PRIMARY_COLOR);

    // them tu
    Button(44, 65, 26, "F2. THÊM TỪ", POS_LEFT, PRIMARY_COLOR);

    // sua tu
    Button(68, 89, 26, "F3. SỬA TỪ", POS_LEFT, PRIMARY_COLOR);

    //xoa tu
    Button(92, 113, 26, "F4. XOÁ TỪ", POS_LEFT, PRIMARY_COLOR);
}


/* XUAT TINH TRANG DANH SACH TU DIEN */
void OutputStatusDict(Word* arrWord, int length, int max_item, int index_selected)
{
	TextColor(PRIMARY_COLOR);
	
	gotoXY(1, max_item + 6);
	printf("TỪ: %d/%d", index_selected + 1, length);
	
	gotoXY(22, max_item + 6);
	printf("TRANG: %d/%d", (int)std::ceil(index_selected/max_item + 1), (int)std::ceil((double)length/max_item));
	
	TextColor(default_ColorCode);
}


/* MENU TRA TU */
void MenuSearhDict(Word *arrWord, int length)
{
    // so phan tu lon nhat duoc xuat ra
    const int max = 28;

    //toa do mac dinh x khi xuat tu
    const int x = 2;  
    
    //toa do nho nhat cua y khi xuat tu
    const int min_y = 5;  

    //toa do lon nhat cua y khi xuat tu
    const int max_y = max + min_y - 1;

    // luu tu tim kiem
    char key[100];
    strcpy(key, "");

    // khoi tao danh sach hien thi
    Word *arrDisplay = new Word[MAX_ARR];
    int length_arrDisplay = length;
    arrDisplay = arrWord;

    // luu index duoc chon
    int index_selected = 0;

    // luu gia tri vi tri xuat
    int start = 0, end, y = min_y;

    // luu trang thai chuong trinh
    int running = 1;

    // luu phim nhan
    KEY KeyPressed = KEY_NONE;         

    RESTART:
    system("cls");

    // tinh gia tri index end
    end = (length_arrDisplay > max) ? start + max : length_arrDisplay;

    // ve khung
    FrameViewDict(0, 40, 0, 34);

    // xuat ra danh sach tu (trong khoang tu start - end)
    OutputListDictKey(arrDisplay, start, end);

    // xuat huong dan su dung
    UserManualMenuSearch();

	// hien thi tinh trang tu dien
	OutputStatusDict(arrDisplay, length_arrDisplay, max, index_selected);
	
    // hien thi tu duoc chon den
    if (length_arrDisplay > 0)
        WordSelected(x, y, arrDisplay[index_selected], 1);

    // hien thi key tim kiem vua moi nhap
    if (strlen(key) > 0)
    {
        gotoXY(11, 2);
        puts(key);
    }

    while (running)
    {
        // nhan phim nhan
        KeyPressed = GetInputKey();

        // xu ly phim nhan
        switch (KeyPressed)
        {
            // PHIM LEN
            case KEY_UP:
                if (index_selected > 0)
                {
                    // xoa select cu
                    WordSelected(x, y, arrDisplay[index_selected], 0);
                    index_selected--;
                    y--;

					OutputStatusDict(arrDisplay, length_arrDisplay, max, index_selected);

                    // quay tro lai 1 trang
                    if (y < min_y)
                    {
                        start -= max;
                        y = max_y;
                        goto RESTART;
                    }

                    //ve select moi
                    WordSelected(x, y, arrDisplay[index_selected], 1);
                }
                break;
            
            // PHIM XUONG
            case KEY_DOWN:
                if (index_selected < length_arrDisplay - 1)
                {
                    WordSelected(x, y, arrDisplay[index_selected], 0);
                    index_selected++;
                    y++;

					OutputStatusDict(arrDisplay, length_arrDisplay, max, index_selected);

                    // het 1 trang
                    if (y > max_y)
                    {
                        start = end;
                        y = min_y;
                        goto RESTART;
                    }
                    WordSelected(x, y, arrDisplay[index_selected], 1);
                }
                break;
            
            // PHIM TIM KIEM
            case KEY_SEARCH:
                fflush(stdin);
                fflush(stdout);
                
                gotoXY(0, 35);
				system("pause");
				// xoa tu cu
                for (int i = 0; i < 32; i++)
                {
                    gotoXY(i, 35);
                    putch(' ');
                }

                // xoa khung select
                WordSelected(x, y, arrDisplay[index_selected], 0);

                // xoa tu cu
                for (int i = 0; i < strlen(key); i++)
                {
                    gotoXY(11 + i, 2);
                    putch(' ');
                }

                gotoXY(11, 2);
                gets(key);

                // tim tu va gan vao mang 
                arrDisplay = Search(arrWord, length, key, length_arrDisplay);

                // neu mang tim kiem duoc khong co phan tu nao thi xuat thong bao
                // tro ve trang thai ban dau
                if (length_arrDisplay == 0)
                {
                    arrDisplay = arrWord;
                    length_arrDisplay = length;
                    int s = ShowMessageBox("Khong tim thay tu", "Thong bao", 0);
                }
                    
                index_selected = 0;
                y = min_y;
                start = 0;
                fflush(stdin);
                goto RESTART;
                break;

            // PHIM THOAT
            case KEY_EXIT:
                running = 0;
                break;

            // PHIM RONG
            case KEY_NONE:
                break;
        }   
        
        KeyPressed = KEY_NONE;
        gotoXY(0, 34);
        Sleep(TIME_DELAY);
    }
}


/* NHAP TU MOI*/
Word InputWord()
{
    fflush(stdin);
    gotoXY(0, 31);
    system("pause");

    gotoXY(0, 31);
    for (int i = 0; i < 110; i++)
        putch(' ');
    
    // khoi tao tu rong
    Word word = {"", "", ""};

    gotoXY(0, 31);
    fflush(stdin);
    printf(">> Nhập từ: ");
    gets(word.key);

    fflush(stdin);
    printf(">> Nhập từ loại: ");
    gets(word.type);

    printf(">> Nhập nghĩa (nhập 0 để dừng):\n");
    char meaning[500];
    int i = 1;
    do
    {
        fflush(stdin);
        printf("\t>> Nhập nghĩa thứ %d: ", i++);
        gets(meaning);
        if (stricmp(meaning, "0") == 0)
            break;

        strcat(word.meaning, meaning);
        strcat(word.meaning, "-");
    }while (stricmp(meaning, "0") != 0);

    return word;
}


/* MENU CHINH SUA DATABASE */
void MenuEditDatabase(Word *arrWord, int &length)
{
    // so luong phan tu max xuat ra
    const int max = 22;

    // toa do mac dinh x khi xuat tu
    const int x = 2;   

    //toa do nho nhat y khi xuat tu
    const int min_y = 5;    

    // toa do lon nhat y khi xuat tu
    const int max_y = max + min_y - 1; 

    int index;

    // luu tu khoa tim kiem
    char key[100];
    strcpy(key, "");
    Word word;

    // khoi tao danh sach hien thi
    Word *arrDisplay = new Word[MAX_ARR];
    int length_arrDisplay = length;
    arrDisplay = arrWord;

    // luu index duoc chon den
    int index_selected = 0;

    // luu vi tri xuat ra tu start - end
    int start = 0, end, y = min_y;

    // luu tru trang thai chuong trinh
    int running = 1;

    // luu tru phim nhan
    KEY KeyPressed; 

    RESTART:
    system("cls");

    // tinh index end xuat ra
    end = (length_arrDisplay > max) ? start + max : length_arrDisplay;

    // ve khung
    FrameViewDict(0, 40, 0, 28);

    // xuat ra danh tu (trong khoang tu start - end)
    OutputListDictKey(arrDisplay, start, end);

    // xuat huong dan su dung
    UserManualMenuEdit();

    OutputStatusDict(arrDisplay, length_arrDisplay, max, index_selected);

    // hien thi khung chon
    if (length_arrDisplay > 0)
        WordSelected(x, y, arrDisplay[index_selected], 1);

    // hien thi key tim kiem vua moi nhap
    if (strlen(key) > 0)
    {
        gotoXY(11, 2);
        puts(key);
    }

    while (running)
    {
        // nhan phim nhan
        KeyPressed = GetInputKey();

        // xu ly phim nhan
        switch (KeyPressed)
        {
            // PHIM LEN
            case KEY_UP:
                if (index_selected > 0)
                {
                    // xoa khung duoc chon hien tai
                    WordSelected(x, y, arrDisplay[index_selected], 0);
                    index_selected--;
                    y--;

                    OutputStatusDict(arrDisplay, length_arrDisplay, max, index_selected);

                    // quay tro ve 1 trang
                    if (y < min_y)
                    {
                        start -= max;
                        y = max_y;
                        goto RESTART;
                    }
                    WordSelected(x, y, arrDisplay[index_selected], 1);
                }
                break;

            // PHIM XUONG
            case KEY_DOWN:
                if (index_selected < length_arrDisplay - 1)
                {
                    // xoa bo select cu
                    WordSelected(x, y, arrDisplay[index_selected], 0);
                    index_selected++;
                    y++;

                    OutputStatusDict(arrDisplay, length_arrDisplay, max, index_selected);

                    // het 1 trang 
                    if (y > max_y)
                    {
                        start = end;
                        y = min_y;
                        goto RESTART;
                    }
                    WordSelected(x, y, arrDisplay[index_selected], 1);
                }
                break;

            // PHIM TIM KIEM
            case KEY_SEARCH:
                fflush(stdin);

				gotoXY(0, 29);
				system("pause");
				// xoa tu cu
                for (int i = 0; i < 32; i++)
                {
                    gotoXY(i, 29);
                    putch(' ');
                }

                //xoa khung select
                WordSelected(x, y, arrDisplay[index_selected], 0);

                // xoao tu tim kiem cu
                for (int i = 0; i < strlen(key); i++)
                {
                    gotoXY(11 + i, 2);
                    putch(' ');
                }

                gotoXY(11, 2);
                gets(key);

                // tim kiem danh sach tu 
                arrDisplay = Search(arrWord, length, key, length_arrDisplay);

                // neu khong tim thay tu nao thi tro ve trang thai ban dau
                if (length_arrDisplay == 0)
                {
                    arrDisplay = arrWord;
                    length_arrDisplay = length;
                    ShowMessageBox("Khong tim thay tu", "Thong bao", 0);
                }

                // neu tim thay tu thi cho hien thi danh sach tu phu hop     
                index_selected = 0;
                y = min_y;
                start = 0;
                fflush(stdin);
                goto RESTART;
                break;

            // PHIM THEM TU
            case KEY_ADD:
                // kiem tra mang day
                if (length >= MAX_ARR)
                {
                    ShowMessageBox("Mang day", "Thong bao", 0);
                    break;
                }

                gotoXY(0, 30);
                printf("(+) THÊM TỪ MỚI\n");

                // lam sach 
                for (int i = 0; i < 90; i++)
                {
                    gotoXY(i, 31);
                    putch(' ');
                }

                // nhap tu
                word = InputWord();
                strcat(word.key, "\n");

                // kiem tra tu bi trung
                index = SearchIndex(arrWord, length, word.key);
                if (index != -1)
                {
                    if (ShowMessageBox("Tu ban them da co trong tu dien\nBan co muon ghi de len khong ?", "Canh bao", 1) == 1)
                        arrWord[index] = word;
                }
                else arrWord[length++] = word;

                // sap xep - ghi file - doc lai data
                Sort(arrWord, length);
                SaveData(arrWord, length);
                ReadData(arrWord, length);

                // khoi tao lai va hien thi danh sach
                if (strlen(key) > 0)
                {
                    arrDisplay = Search(arrWord, length, key, length_arrDisplay);

                    if (length_arrDisplay == 0)
                    {
                        arrDisplay = arrWord;
                        length_arrDisplay = length;
                    }
                }
                else 
                {
                    arrDisplay = arrWord;
                    length_arrDisplay = length;
                }

                start = 0;
                y = min_y;
                index_selected = 0;

                goto RESTART;
                break;

            // PHIM SUA TU
            case KEY_EDIT:
                gotoXY(0, 30);
                printf("(+) SỬA TỪ\n");

                arrWord[(BinarySearch(arrWord, arrDisplay[index_selected].key, 0, length - 1))] = InputWord();
                
                // sap xep - luu - doco lai data
                Sort(arrWord, length);
                SaveData(arrWord, length);
                ReadData(arrWord, length);

                // neu nhu co tim kiem thi khoi tao lai chuc nang tim kiem
                if (strlen(key) > 0)
                {
                    arrDisplay = Search(arrWord, length, key, length_arrDisplay);

                    if (length_arrDisplay == 0)
                    {
                        arrDisplay = arrWord;
                        length_arrDisplay = length;
                    }
                }
                else 
                {
                    arrDisplay = arrWord;
                    length_arrDisplay = length;
                }

                y = min_y;
                start = 0;
                index_selected = 0;

                goto RESTART;
                break;
            
            // PHIM XOA TI
            case KEY_DELETE:
                if (ShowMessageBox("Ban co muon xoa tu nay khong ?", "Canh bao", 1) == 1)
                {
                    Delete(arrWord, length, arrDisplay[index_selected].key);

                     // sap xep - luu - doco lai data
                    Sort(arrWord, length);
                    SaveData(arrWord, length);
                    ReadData(arrWord, length);

                    // neu nhu co tim kiem thi khoi tao lai chuc nang tim kiem
                    if (strlen(key) > 0)
                    {
                        arrDisplay = Search(arrWord, length, key, length_arrDisplay);

                        if (length_arrDisplay == 0)
                        {
                            arrDisplay = arrWord;
                            length_arrDisplay = length;
                        }
                    }
                    else 
                    {
                        arrDisplay = arrWord;
                        length_arrDisplay = length;
                    }

                    goto RESTART;
                }
                break;
            
            // PHIM THOAT
            case KEY_EXIT:
                running = 0;
                break;

            // KHONG NHAN PHIM NAO
            case KEY_NONE:
                break;
        }
        
        KeyPressed = KEY_NONE;
        gotoXY(0, 34);
        Sleep(TIME_DELAY);
    }
}


/* CHUC NANG GIOI THIEU */
void Introduce()
{
    // gia tri y bat dau
    int y = 21;

    while (y > 1)
    {
        system("cls");
        gotoXY(49, y);
        printf("Bài tập nhóm 13");

        gotoXY(49, y + 1);
        printf("Lớp: ĐHKTPM13ATT");

        gotoXY(44, y + 2);
        printf("Đề tài: Từ điển Anh - Việt");

        gotoXY(49, y + 5);
        printf("THÀNH VIÊN NHÓM");

        gotoXY(30, y + 6);
        printf("%-12s%-30s%-30s", "MSSV", "Họ và tên", "  Công việc");

        gotoXY(30, y + 7);
        printf("-------------------------------------------------------------");

        gotoXY(30, y + 8);
        printf("%-12s%-30s%-30s", "17010481", "Nguyễn Chí Cường", "    Giao diện");

        gotoXY(30, y + 9);
        printf("%-12s%-30s%-30s", "17023061", "Phạm Minh Khoa", "Trưởng nhóm + Tester");

        gotoXY(30, y + 10);
        printf("%-12s%-30s%-30s", "17082171", "Bùi Văn Ninh", "Chương trình chính");

        gotoXY(40, y + 14);
        printf("Copyright Nhóm 13 - ĐHKTPM13ATT - IUH");
        y--;
        
        Sleep(150);
    }

    // tro ve man hinh chinh
    gotoXY(40, 34);
    printf("Nhấn phím ESC để trở về màn hình chính");
    while (GetInputKey() != KEY_EXIT);
}
