#include <iostream>
#include <vector>
using namespace std;

/* �ռ� UI ���� */

class Card;
class Account;
class Bank;
class ATM;
class Session;

class Card {
private:
    int acc_num; // ���¹�ȣ
    Bank* card_bank; // ī�� ����
    int card_num; // ī�� ��ȣ
    string user_name; // ī�� ����� �̸�
public:
    Card(int account_num, Bank* bank, int card_number, string name) {
        acc_num = account_num;
        card_bank = bank;
        card_num = card_number;
        user_name = name;
    }; //Card Constructor
    int get_acc_number() { return acc_num; }
    int get_card_number() { return card_num; }
    Bank* get_CardBank() { return card_bank; }
    string get_user_name() { return user_name; }
};

class Account {
private:
    Bank* bank; // ���¿� ������ ����
    int acc_num; // ���¹�ȣ 12�ڸ�
    int password; // ��й�ȣ
    int Acash; // ���º�����
public:
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

class Bank {
private:
    string bank_name; // ���� �̸�
    vector<Account*> account_lst; // ���� ���� ���� ���
    int numOfuser = 0; // user account ����
    static vector<Bank*> bank_list;
public:
    Bank(string bname) {
        bank_name = bname;
        bank_list.push_back(this);
    };
    string get_bank_name() { return bank_name; }; // ��ũ �̸� ��ȯ
    void add_accountlst(Account* new_account) { account_lst.push_back(new_account); }; // ���� ������ �߰�
    Account* find_account(int acc_num) {
        vector<Account*>::iterator iter;
        for (iter = account_lst.begin(); iter != account_lst.end(); ++iter) {
            if ((*iter)->get_accountnum() == acc_num) {
                return (*iter);
            }
        }
        return nullptr; // Move this line outside of the loop
    }

    Account* find_trans_account(int acc_num) {
        vector<Bank*>::iterator iter_bank;
        vector <Account*>::iterator iter_account;
        for (iter_bank = bank_list.begin(); iter_bank != bank_list.end(); ++iter_bank) {
            for (iter_account = (*iter_bank)->account_lst.begin(); iter_account != (*iter_bank)->account_lst.end(); ++iter_account) {
                if ((*iter_account)->get_accountnum() == acc_num) {
                    return (*iter_account);
                }
            }
        }
        return nullptr; // Move this line outside of the loop
    }
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
    static int withdrawal_num;
    string session;
public:
    ATM(Bank* mbank, int sernum, string type, string lang, int num_1000, int num_5000, int num_10000, int num_50000) {
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
        if (lang_type == "Bi") {
            cout << "1. English 2. Korean" << endl;
            cin >> lang_type;
            if (lang_type == "2") {
                cout << "ī�� �־�" << endl;
                if (ATM_type == "Single" && main_bank != card->get_CardBank()) {
                    cout << "��ȿ���� ���� ī��" << endl;
                    return;
                }
                else {
                    enter_password(card);
                    return;
                }
                return;
            }
            else if (lang_type == "1") { ; }
        }

        else if (lang_type == "Uni" || lang_type == "1") {
            cout << "Insert the Card" << endl;
            if (ATM_type == "Single" && main_bank != card->get_CardBank()) {
                cout << "Invalid Card" << endl;
                return;
            }
            else {
                enter_password(card);
                return;
            }
        }
    };
    int get_acc_password(Card* card) { //���� ��й�ȣ ��ȯ
        int password;
        password = card->get_CardBank()->find_account(card->get_acc_number())->get_password();
        return password;
    }
    void enter_password(Card* card) { // ��й�ȣ �Է� ȭ��
        int password;
        int wrong_time = 0;
        string etp = "Enter the password: ";
        string Ip = "Invalid password";
        string Ttw = "Three time wrong";
        string Fa = "Find account";
        string etp_k = "��� �Է�: ";
        string Ip_k = "Ʋ�� ���";
        string Ttw_k = "�� �� �߸� �Է�";
        string Fa_k = "���¸� ã�ҽ��ϴ�";
        string ep[4];

        if (lang_type == "2") {
            ep[0] = etp_k;
            ep[1] = Ip_k;
            ep[2] = Ttw_k;
            ep[3] = Fa_k;
        }
        else if (lang_type == "Uni" || lang_type == "1") {
            ep[0] = etp;
            ep[1] = Ip;
            ep[2] = Ttw;
            ep[3] = Fa;
        }
        while (true) {
            cout << ep[0];
            cin >> password;
            if (password != get_acc_password(card)) {
                cout << ep[1] << endl;
                wrong_time++;
                if (wrong_time == 3) {
                    cout << ep[2] << endl;
                    return;
                }
            }
            else {
                cout << ep[3] << endl;
                Account* acc = card->get_CardBank()->find_account(card->get_acc_number());
                mainstage(acc);
                break;
            }
        }
    }
    void mainstage(Account* acc) { // Transaction Main
        cout << "1. Deposit" << endl;
        cout << "2. Withdrawal" << endl;
        cout << "3. Transfer" << endl;
        cout << "4. Quit" << endl;
        int input;
        cin >> input;
        if (input == 1) {
            cout << "Deposit" << endl;
            deposit(acc);
            return;
        }
        else if (input == 2) {
            if (withdrawal_num >= 3) {
                cout << "Withdrawal number limit exeeded, restart session for more withdraw" << endl;
                mainstage(acc);
                return;
            }
            cout << "Withdrawal" << endl;
            withdrawal(acc);
            return;
        }
        else if (input == 3) {
            cout << "Transfer" << endl;
            transfer(acc);
            return;
        }
        else if (input == 4) {
            withdrawal_num = 0;
            return;
        }
        else {
            cout << "Invalid number" << endl;
            mainstage(acc);
        }
    }
    void deposit(Account* acc) { // ���� ��� ����
        cout << "1. Cash" << endl;
        cout << "2. Check" << endl;
        cout << "3. Menu" << endl;
        cout << "4. Quit" << endl;
        int deposit_method;
        cin >> deposit_method;
        if (deposit_method == 1) {
            cash_deposit(acc);
            return;
        }
        else if (deposit_method == 2) {
            check_deposit(acc);
            return;
        }
        else if (deposit_method == 3) {
            mainstage(acc);
            return;
        }
        else if (deposit_method == 4) {
            withdrawal_num = 0;
            return;
        }
        else {
            cout << "Invalid number" << endl;
            deposit(acc);
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
        cout << "1000: ";
        cin >> num_1000;
        if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
            cout << "Banknote limit exeeded" << endl;
            mainstage(acc);
            return;
        }
        if (main_bank == acc->get_bank()) {
            cash_50000 += num_50000;
            cash_10000 += num_10000;
            cash_5000 += num_5000;
            cash_1000 += num_1000;
            acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000);
            mainstage(acc);
            return;
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
                mainstage(acc);
                return;
            }
            else {
                cout << "Invalid deposit fee" << endl;
                mainstage(acc);
                return;
            }
        }
    }
    void check_deposit(Account* acc) {
        int check_cash, check_num;
        cout << "Insert cash";
        cin >> check_cash;
        cout << "How many check you will insert?";
        cin >> check_num;
        if (check_num > 30) {
            cout << "Check limit exeeded" << endl;
            mainstage(acc);
            return;
        }
        if (main_bank == acc->get_bank()) {
            acc->update_cash(check_cash * check_num);
            mainstage(acc);
            return;
        }
        else {
            int deposit_fee;
            cout << "Please insert deposit fee 1000 Won" << endl;
            cout << "1000: ";
            cin >> deposit_fee;
            if (deposit_fee == 1) {
                cash_1000 += 1;
                acc->update_cash(check_cash * check_num);
                mainstage(acc);
                return;
            }
            else {
                cout << "Invalid deposit fee" << endl;
                mainstage(acc);
                return;
            }
        }
    }
    void withdrawal(Account* acc) { // ���
        int num_50000, num_10000, num_5000, num_1000;
        cout << "Withdraw cash" << endl;
        cout << "Insert Cash" << endl;
        cout << "50000: ";
        cin >> num_50000;
        if (num_50000 > cash_50000) {
            cout << "ATM Exeeded cash";
            mainstage(acc);
            return;
        }
        cout << "10000: ";
        cin >> num_10000;
        if (num_10000 > cash_10000) {
            cout << "ATM Exeeded cash";
            mainstage(acc);
            return;
        }
        cout << "5000: ";
        cin >> num_5000;
        if (num_5000 > cash_5000) {
            cout << "ATM Exeeded cash";
            mainstage(acc);
            return;
        }
        cout << "1000: ";
        cin >> num_1000;
        if (num_1000 > cash_1000) {
            cout << "ATM Exeeded cash" << endl;
            mainstage(acc);
            return;
        }
        if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 > 500000) {
            cout << "Withdrawal Exeeded cash" << endl;
            mainstage(acc);
            return;
        }
        if (main_bank == acc->get_bank()) {
            if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + 1000 > acc->get_cash()) {
                cout << "Account Exeeded cash" << endl;
                mainstage(acc);
                return;
            }
            cout << 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 << " Won withdrawed" << endl;
            cash_50000 -= num_50000;
            cash_10000 -= num_10000;
            cash_5000 -= num_5000;
            cash_1000 -= num_1000;
            acc->update_cash(-(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + 1000));
            withdrawal_num++;
            mainstage(acc);
            return;
        }
        else {
            if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + 2000 > acc->get_cash()) {
                cout << "Account Exeeded cash" << endl;
                mainstage(acc);
                return;
            }
            cout << 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 << " Won withdrawed" << endl;
            cash_50000 -= num_50000;
            cash_10000 -= num_10000;
            cash_5000 -= num_5000;
            cash_1000 -= num_1000;
            acc->update_cash(-(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + 2000));
            withdrawal_num++;
            mainstage(acc);
            return;
        }
    };
    void transfer(Account* acc) { // �۱�
        int transfer_type;
        cout << "Choose transfer type" << endl;
        cout << "1. Cash transfer" << endl;
        cout << "2. Account fund transfer" << endl;
        cout << "3. Menu" << endl;
        cout << "3. Quit" << endl;
        cin >> transfer_type;
        if (transfer_type == 1) {
            cash_transfer(acc);
            return;
        }
        else if (transfer_type == 2) {
            acc_transfer(acc);
            return;
        }
        else if (transfer_type == 3) {
            mainstage(acc);
            return;
        }
        else if (transfer_type == 4) {
            withdrawal_num = 0;
            return;
        }
        else {
            cout << "Invalid number" << endl;
            transfer(acc);
            return;
        }
    };
    void cash_transfer(Account* acc) {
        int trans_acc_num;
        cout << "Input account number for transfer: ";
        cin >> trans_acc_num;
        Account* trans_acc = main_bank->find_trans_account(trans_acc_num);
        if (trans_acc == nullptr) {
            cout << "Invalid account number" << endl;
            mainstage(acc);
            return;
        }
        int num_50000, num_10000, num_5000, num_1000;
        cout << "Insert Cash including transfer fee 5000 Won" << endl;
        cout << "50000: ";
        cin >> num_50000;
        cout << "10000: ";
        cin >> num_10000;
        cout << "5000: ";
        cin >> num_5000;
        cout << "1000: ";
        cin >> num_1000;
        if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
            cout << "Banknote limit exeeded" << endl;
            mainstage(acc);
            return;
        }
        cash_50000 += num_50000;
        cash_10000 += num_10000;
        cash_5000 += num_5000;
        cash_1000 += num_1000;
        acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 - 5000);
        mainstage(acc);
        return;
    }
    void acc_transfer(Account* acc) {
        int trans_acc_num;
        cout << "Input account number for transfer: ";
        cin >> trans_acc_num;
        Account* trans_acc = main_bank->find_trans_account(trans_acc_num);
        if (trans_acc == nullptr) {
            cout << "Invalid account number" << endl;
            mainstage(acc);
            return;
        }
        int fund;
        cout << "Amount of fund to transfer: " << endl;
        cin >> fund;
        if (acc->get_bank() == main_bank && trans_acc->get_bank() == main_bank) {
            if (fund + 2000 > acc->get_cash()) {
                cout << "Account exeeded cash" << endl;
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + 2000));
            trans_acc->update_cash(fund);
            mainstage(acc);
        }
        else if (acc->get_bank() != main_bank && trans_acc->get_bank() != main_bank) {
            if (fund + 4000 > acc->get_cash()) {
                cout << "Account exeeded cash" << endl;
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + 4000));
            trans_acc->update_cash(fund);
            mainstage(acc);
            return;
        }
        else {
            if (fund + 3000 > acc->get_cash()) {
                cout << "Account exeeded cash" << endl;
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + 3000));
            trans_acc->update_cash(fund);
            mainstage(acc);
            return;
        }
    }
    void update_session(string history) {
        session += history;
        session += "\n";
    }
};

vector<Bank*> Bank::bank_list;
int ATM::withdrawal_num = 0;


int main() {
    Bank* wooribank = new Bank("woori");
    Account* acc1 = new Account(wooribank, 12345678, 0000, 1000000);
    wooribank->add_accountlst(acc1);
    Card* card1 = new Card(12345678, wooribank, 987, "Jisang");
    ATM* atm1 = new ATM(wooribank, 111111111, "Single", "Uni", 100, 100, 100, 100);

    Bank* daegubank = new Bank("daegu");
    ATM* atm2 = new ATM(daegubank, 22222222, "Multi", "Bi", 100, 100, 100, 100);
    
    Bank* admin = new Bank("admin");  //�ϴ� �ӽ÷� �̷��� �س���. ���� ����غ��ô�.
    Card* ad_card = new Card(1111, admin, 2222, "admin");

    atm1->start_ATM(card1);

    atm2->start_ATM(card1);

}
