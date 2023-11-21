//su-geong-haetdda~

#include <iostream>
using namespace std;

/* 손수 UI 구성 */

class ATM;
class Bank;
class User;
class Account;
class Session;
class Card;
class UI;
class Admin;

class Card {
private:
    int acc_num; // 계좌번호
    Bank* card_bank; // 카드 은행
    int card_num; // 카드 번호
    string user_name; // 카드 사용자 이름
public:
    Card() {};
    Card(int account_num, Bank* bank, int card_number) {
        acc_num = account_num;
        card_bank = bank;
        card_num = card_number;
    }; //Card Constructor
    int get_acc_serial() { return acc_num; }
    int get_card_serial() { return card_num; }
    Bank* get_CardBank() { return card_bank; }
};

class Bank {
private:
    string bank_name; // 은행 이름
    Account* account_lst[100]; // 은행 보유 계좌 목록
    int numOfuser = 0; // user acount 개수
public:
    Bank() {};
    Bank(string bname) { bank_name = bname; };
    string get_bank_name() { return bank_name; }; // 뱅크 이름 반환
    void add_accountlst(Account* new_account) { account_lst[numOfuser++] = new_account; }; // 계좌 생성시 추가
    Account* find_account(int account_num) { // 계좌 번호에 맞는 계좌 반환
        for (int i = 0; i < numOfuser; i++) {
            if (account_lst[i]->get_accountnum() == account_num) {
                return account_lst[i];
            }
        }
        return 0;
    };
};

class Account {
private:
    Bank* bank; // 계좌에 연동된 은행
    int acc_num; // 계좌번호 12자리
    int password; // 비밀번호
    int Acash; // 계좌보유금
public:
    Account() {}; //계좌설정
    Account(Bank* bname, int anum, int pw, int ac) {
        bank = bname;
        acc_num = anum;
        password = pw;
        Acash = ac;
    }; // 계좌 초기 설정
    void update_cash(int inoutcash) { Acash += inoutcash; }; // 계좌보유금 변경
    int get_password() { return password; }; // 비밀번호 반환
    Bank* get_bank() { return bank; }; // 은행 반환
    int get_accountnum() { return acc_num; }; //계좌번호 반환
    int get_cash() { return Acash; }; //남은 현금 반환
};

class ATM { // Unilingual Multi Bank 기준
private:
    Bank* main_bank; // 주 은행 설정
    int ATM_serial; // 고유 넘버 6자리
    string ATM_type; // 단일 은행과 다중 은행으로 구분. 단일 은행은 주 은행 카드만 받음
    string lang_type; // 단일 언어, 이중 언어(한영)
    int cash_1000; // ATM 1000원짜리 현금
    int cash_5000; // ATM 5000원짜리 현금
    int cash_10000; // ATM 10000원짜리 현금
    int cash_50000; // ATM 50000원짜리 현금
    Bank* Blist; // Bank 리스트 input
    string session;
public:
    ATM() {}; // 디폴트값 설정
    ATM(Bank* mbank, int sernum, string type, string lang, int num_1000, int num_5000, int num_10000, int num_50000, Bank* Blist) {
        main_bank = mbank;
        ATM_serial = sernum;
        ATM_type = type;
        lang_type = lang;
        cash_1000 = num_1000;
        cash_5000 = num_5000;
        cash_10000 = num_10000;
        cash_50000 = num_50000;
    }; // ATM 초기 설정
    Bank* get_ATMBank() { return main_bank; } // ATM의 primiary bank 반환
    int get_serial() { return ATM_serial; } // ATM의 일련번호 반환
    string get_ATM_type() { return ATM_type; } // ATM의 type 반환 (Multi or Single)
    string get_lang_type() { return lang_type; } // ATM의 language type 반환 (Unilingual or Bilingual)
    void start_ATM(Card* card) { // ATM 시작화면
        cout << "Insert the Card" << endl;
        if (this->get_ATM_type() == "Single" && this->get_ATMBank() != card->get_CardBank()) {
            cout << "Invalid Card" << endl;
            return;
        }
        else {
            enter_password(card);
            return;
        }
    };
    int get_acc_password(Card* card) { //계좌 비밀번호 반환
        int password;
        password = card->get_CardBank()->find_account(card->get_card_serial())->get_password();
        return password;
    }
    void enter_password(Card* card) { // 비밀번호 입력 화면
        int password;
        int wrong_time = 0;
        cout << "Enter the password: ";
        while (true) {
            cin >> password;
            if (password != get_acc_password(card)) {
                cout << "Invalid password" << endl;
                wrong_time++;
                if (wrong_time == 3) {
                    cout << "Three time wrong" << endl;
                    start_ATM(card);
                    return;
                }
            }
            else {
                Account* acc = card->get_CardBank()->find_account(card->get_card_serial());
                mainstage(acc);
                return;
            }
        }
    }
    void mainstage(Account* acc) { // Transaction Main
        cout << "1. Deposit" << endl;
        cout << "2. Withdrawal" << endl;
        cout << "3. Transfer" << endl;
        cout << "4. Quit" << endl;
        int input = 0;
        cin >> input;
        if (input == 1) {
            cout << "Deposit" << endl;
            deposit();
            return;
        }
        else if (input == 2) {
            cout << "Withdrawal" << endl;
            withdrawal();
            return;
        }
        else if (input == 3) {
            cout << "Transfer" << endl;
            transfer();
            return;
        }
        else if (input == 4) {
            return;
        }
        else {
            cout << "Invalid number" << endl;
        }
    }
    void deposit(Account* acc) { // 예금 방법 선택
        cout << "1. Cash" << endl;
        cout << "2. Check" << endl;
        cout << "3. Quit" << endl;
        int deposit_method;
        cin >> deposit_method;
        switch (deposit_method) {
        case 1:
            cash_deposit(acc);
            return;
        case 2:
            check_deposit(acc);
            return;
        case 3:
            return;
        }
    };
    void cash_deposit(Account* acc) {
        int num_50000, num_10000, num_5000, num_1000;
        cout << "Insert Cash" << endl;
        cout << "50000: ";
        cin >> num_50000;
        cout << "10000: ";
        cin >> num_10000;
        cout << "5000: ";
        cin >> num_5000;
        cout << "1000";
        cin >> num_1000;
        if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
            cout << "Banknote limit exeeded" << endl;
            cash_deposit(acc);
            return;
        }
        if (acc->get_bank() == this->get_ATMBank()) {
            cash_50000 += num_50000;
            cash_10000 += num_10000;
            cash_5000 += num_5000;
            cash_1000 += num_1000;
            acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);
        }
        else {
            int deposit_fee;
            cout << "Please insert deposit fee 1000 Won" << endl;
            cout << "1000: ";
            cin >> deposit_fee;
            if (deposit_fee == 1) {
                cash_50000 += num_50000;
                cash_10000 += num_10000;
                cash_5000 += num_5000;
                cash_1000 += num_1000 + 1;
                acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);

            }
            else {
                cout << "Invalid deposit fee" << endl;
                cash_deposit(acc);
                return;
            }
        }
    }
    void check_deposit(Account* acc) {

    }
    void withdrawal(int cash, string paper) {
        int caash = 0;
        cin >> caash;
        if (caash < 1000) {
            cout << "완료 화면" << endl;
        }
    }; // 출금
    void transfer(int cash, string bank_name, int account_num) {}; // 송금, 맞는 계좌로 돈을 송금. 아니면 문구 내고 ending ui
    int chek_fee(int cash, int type) {}; //수수료 계산, 예금 출금 송금시 알맞은 수수료 계산 실행
    void update_session(string history) {
        session += history;
        session += "\n"
    }
};

class User {
private:
    string user_name; // 사용자 이름
    Account* Uaccount_lst[100]; // 사용자의 계좌 목록
public:
    User() {};
    User(string uname, Account* ulst) {};
};

class Session {
private:
    static int identifier; // 거래 식별자
    string breakdown; // 거래 내역
public:
    Session() {};
    Session(int idf) {}; // 식별자가 담긴 Session생성
    void updateTransaction(string BD) {
        breakdown += BD;
        breakdown += "\n";
    }; // 거래내역 업데이트
    void end_Session() {}; // 세션 종료, 입력이나 예외 상황에 따라 end_Session 실행
    ~Session() {};
};

class UI {
private:
    //ATM* atm_type;
    string language_type;
    ATM* atm;
    Card* card;
    int error_num;
    Session* now_Session;
public:
    UI() {}; // 처음 설정
    UI(string lang) {}; // 언어 설정

    /* 끝나는 케이스엔 엔딩 함수 출력*/

    void start_ui(ATM* atm, Card* card) {
        if (atm->get_ATM_type() == "Multi" && atm->get_ATMBank() != card->get_CardBank()) {
            cout << "Invalid Card" << endl;
            return;
        }
        if (card->get_Serial() == atm->get_Serial()) {
            admin_statement();
            return;
        }
        if (atm->get_lang_type() == "Bilingual") {
            set_lang();
            return;
        }
        input_password();
    }; // 초기화면 ui 설정, atm 종류도 받음, new session 생성하고 이것을 now_Session으로 지정
    void set_lang() {}; // atm_type에 맞는 언어 실행. 다중언어인 경우 선택지를 선택할 수 있게 설정
    void input_card() {}; // 카드 입력시 output 출력
    void input_password() {
        int password;
        cin >> password;
        if (password == card->get_CardBank()->)
    }; // 비밀번호를 입력받는 함수
    void main_stage() {}; // 선택지를 주는 UI
    void deposit() {}; // 예금에 필요한 돈을 입력받음, 수표인지 아닌지
    void withdrawal() {}; // 출금에 필요한 돈을 입력받음, 수표인지 아닌지
    void transfer() {}; // 송금에 필요한 돈을 입력받음
    void error_password() {}; // 비번에러
    void admin_statement() {}; // admincard 입력시 출력창
    void ending() {}; // 마무리 출력, 세션을 종료하고 adminsession에 추가
};





int main() {
    Bank* wooribank = new Bank("woori");
    Account* acc1 = new Account(wooribank, 12345678, "0000", 1000);
    wooribank->add_accountlst(acc1);
    Card* card1 = new Card(acc1);
    ATM* atm1 = new ATM(wooribank, 0001, "single", "korean", 10000);

    atm1->start_ATM(card1);
}
