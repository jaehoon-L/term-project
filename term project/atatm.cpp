//su-geong-haetdda~

#include <iostream>
using namespace std;

/* �ռ� UI ���� */

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
    int acc_num; // ���¹�ȣ
    Bank* card_bank; // ī�� ����
    int card_num; // ī�� ��ȣ
    string user_name; // ī�� ����� �̸�
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
    string bank_name; // ���� �̸�
    Account* account_lst[100]; // ���� ���� ���� ���
    int numOfuser = 0; // user acount ����
public:
    Bank() {};
    Bank(string bname) { bank_name = bname; };
    string get_bank_name() { return bank_name; }; // ��ũ �̸� ��ȯ
    void add_accountlst(Account* new_account) { account_lst[numOfuser++] = new_account; }; // ���� ������ �߰�
    Account* find_account(int account_num) { // ���� ��ȣ�� �´� ���� ��ȯ
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
    Bank* bank; // ���¿� ������ ����
    int acc_num; // ���¹�ȣ 12�ڸ�
    int password; // ��й�ȣ
    int Acash; // ���º�����
public:
    Account() {}; //���¼���
    Account(Bank* bname, int anum, int pw, int ac) {
        bank = bname;
        acc_num = anum;
        password = pw;
        Acash = ac;
    }; // ���� �ʱ� ����
    void update_cash(int inoutcash) { Acash += inoutcash; }; // ���º����� ����
    int get_password() { return password; }; // ��й�ȣ ��ȯ
    Bank* get_bank() { return bank; }; // ���� ��ȯ
    int get_accountnum() { return acc_num; }; //���¹�ȣ ��ȯ
    int get_cash() { return Acash; }; //���� ���� ��ȯ
};

class ATM { // Unilingual Multi Bank ����
private:
    Bank* main_bank; // �� ���� ����
    int ATM_serial; // ���� �ѹ� 6�ڸ�
    string ATM_type; // ���� ����� ���� �������� ����. ���� ������ �� ���� ī�常 ����
    string lang_type; // ���� ���, ���� ���(�ѿ�)
    int cash_1000; // ATM 1000��¥�� ����
    int cash_5000; // ATM 5000��¥�� ����
    int cash_10000; // ATM 10000��¥�� ����
    int cash_50000; // ATM 50000��¥�� ����
    Bank* Blist; // Bank ����Ʈ input
    string session;
public:
    ATM() {}; // ����Ʈ�� ����
    ATM(Bank* mbank, int sernum, string type, string lang, int num_1000, int num_5000, int num_10000, int num_50000, Bank* Blist) {
        main_bank = mbank;
        ATM_serial = sernum;
        ATM_type = type;
        lang_type = lang;
        cash_1000 = num_1000;
        cash_5000 = num_5000;
        cash_10000 = num_10000;
        cash_50000 = num_50000;
    }; // ATM �ʱ� ����
    Bank* get_ATMBank() { return main_bank; } // ATM�� primiary bank ��ȯ
    int get_serial() { return ATM_serial; } // ATM�� �Ϸù�ȣ ��ȯ
    string get_ATM_type() { return ATM_type; } // ATM�� type ��ȯ (Multi or Single)
    string get_lang_type() { return lang_type; } // ATM�� language type ��ȯ (Unilingual or Bilingual)
    void start_ATM(Card* card) { // ATM ����ȭ��
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
    int get_acc_password(Card* card) { //���� ��й�ȣ ��ȯ
        int password;
        password = card->get_CardBank()->find_account(card->get_card_serial())->get_password();
        return password;
    }
    void enter_password(Card* card) { // ��й�ȣ �Է� ȭ��
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
    void deposit(Account* acc) { // ���� ��� ����
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
            cout << "�Ϸ� ȭ��" << endl;
        }
    }; // ���
    void transfer(int cash, string bank_name, int account_num) {}; // �۱�, �´� ���·� ���� �۱�. �ƴϸ� ���� ���� ending ui
    int chek_fee(int cash, int type) {}; //������ ���, ���� ��� �۱ݽ� �˸��� ������ ��� ����
    void update_session(string history) {
        session += history;
        session += "\n"
    }
};

class User {
private:
    string user_name; // ����� �̸�
    Account* Uaccount_lst[100]; // ������� ���� ���
public:
    User() {};
    User(string uname, Account* ulst) {};
};

class Session {
private:
    static int identifier; // �ŷ� �ĺ���
    string breakdown; // �ŷ� ����
public:
    Session() {};
    Session(int idf) {}; // �ĺ��ڰ� ��� Session����
    void updateTransaction(string BD) {
        breakdown += BD;
        breakdown += "\n";
    }; // �ŷ����� ������Ʈ
    void end_Session() {}; // ���� ����, �Է��̳� ���� ��Ȳ�� ���� end_Session ����
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
    UI() {}; // ó�� ����
    UI(string lang) {}; // ��� ����

    /* ������ ���̽��� ���� �Լ� ���*/

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
    }; // �ʱ�ȭ�� ui ����, atm ������ ����, new session �����ϰ� �̰��� now_Session���� ����
    void set_lang() {}; // atm_type�� �´� ��� ����. ���߾���� ��� �������� ������ �� �ְ� ����
    void input_card() {}; // ī�� �Է½� output ���
    void input_password() {
        int password;
        cin >> password;
        if (password == card->get_CardBank()->)
    }; // ��й�ȣ�� �Է¹޴� �Լ�
    void main_stage() {}; // �������� �ִ� UI
    void deposit() {}; // ���ݿ� �ʿ��� ���� �Է¹���, ��ǥ���� �ƴ���
    void withdrawal() {}; // ��ݿ� �ʿ��� ���� �Է¹���, ��ǥ���� �ƴ���
    void transfer() {}; // �۱ݿ� �ʿ��� ���� �Է¹���
    void error_password() {}; // �������
    void admin_statement() {}; // admincard �Է½� ���â
    void ending() {}; // ������ ���, ������ �����ϰ� adminsession�� �߰�
};





int main() {
    Bank* wooribank = new Bank("woori");
    Account* acc1 = new Account(wooribank, 12345678, "0000", 1000);
    wooribank->add_accountlst(acc1);
    Card* card1 = new Card(acc1);
    ATM* atm1 = new ATM(wooribank, 0001, "single", "korean", 10000);

    atm1->start_ATM(card1);
}
