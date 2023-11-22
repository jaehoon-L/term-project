#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

/* 손수 UI 구성 */

class Account;
class Bank;
class Single_ATM;
class Multi_ATM;
class Trans_History;

class Trans_History {
private:
	static int id_counter;
	int trans_id;
	int cardnumber;
	int accountnumber;
	string trans_type;
	int amount;
	int other_account;
public:
	int get_trans_id() { return trans_id; }
	int get_cardnumber() { return cardnumber; }
	int get_accountnumber() { return accountnumber; }
	string get_trans_type() { return trans_type; }
	int get_amount() { return amount; }
	int get_other_acc() { return other_account; }
	Trans_History(int cnum, int anum, string type, int money, int othacc) {
		cardnumber = cnum;
		accountnumber = anum;
		trans_type = type;
		amount = money;
		other_account = othacc;
		trans_id = ++id_counter;
	}
};

class Account {
private:
    Bank* bank; // 계좌에 연동된 은행
    int acc_num; // 계좌번호 12자리
    int password; // 비밀번호
    int Acash; // 계좌보유금
    int card_num; // 계좌의 카드번호
    string user_name; // 계좌 사용자 이름
public:
    Account(Bank* bname, int anum, int pw, int ac, int cn, string un);
    void update_cash(int inoutcash) { Acash += inoutcash; }; // 계좌보유금 변경
    int get_password() { return password; }; // 비밀번호 반환
    Bank* get_bank() { return bank; }; // 은행 반환
    int get_accountnum() { return acc_num; }; //계좌번호 반환
    int get_cash() { return Acash; }; //남은 현금 반환
    int get_card_num() { return card_num;} // 카드 번호 반환
    string get_name() { return user_name; }
};

class Bank {
private:
    string bank_name; // 은행 이름
    vector<Account*> account_lst; // 은행 보유 계좌 목록
    int numOfuser = 0; // user account 개수
    static vector<Bank*> bank_list;
public:
    Bank(string bname) {
        bank_name = bname;
		addBankToList(this);
	};

	static void addBankToList(Bank* bank) {
		bank_list.push_back(bank);
	}
    string get_bank_name() { return bank_name; }; // 뱅크 이름 반환
    void add_accountlst(Account* new_account) { account_lst.push_back(new_account); }; // 계좌 생성시 추가
	Account* find_account(int card_num) {
		vector<Bank*>::iterator iter_bank;
		vector <Account*>::iterator iter_account;
		for (iter_bank = bank_list.begin(); iter_bank != bank_list.end(); ++iter_bank) {
			for (iter_account = (*iter_bank)->account_lst.begin(); iter_account != (*iter_bank)->account_lst.end(); ++iter_account) {
				if ((*iter_account)->get_card_num() == card_num) {
					return (*iter_account);
				}
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
    Bank* find_bank(string b_name) {
        vector<Bank*>::iterator iter_bank;
        for (iter_bank = bank_list.begin(); iter_bank != bank_list.end(); ++iter_bank) {
            if ((*iter_bank)->get_bank_name() == b_name) {
                return (*iter_bank);
            }
        }
        return nullptr;
    }
    void display_account() {
		vector<Bank*>::iterator iter_bank;
		vector <Account*>::iterator iter_account;
		for (iter_bank = bank_list.begin()+1; iter_bank != bank_list.end(); ++iter_bank) {
			for (iter_account = (*iter_bank)->account_lst.begin(); iter_account != (*iter_bank)->account_lst.end(); ++iter_account) {
                cout << "Account [Bank: " << bank_name << ", No: " << (*iter_account)->get_accountnum() << ", Owner: " << (*iter_account)->get_name() << "] balance: " << (*iter_account)->get_cash() << endl;
			}
		}
    }

	void set_banklist(vector<Bank*> list) {
		bank_list = list;
	}

	vector<Bank*> get_banklist() {
		return bank_list;
	}
};

Account::Account(Bank* bname, int anum, int pw, int ac, int cn, string un) {
	bank = bname;
	acc_num = anum;
	password = pw;
	Acash = ac;
	card_num = cn;
	user_name = un;
	bname->add_accountlst(this);
	cout << "계좌생성) 은행 이름: " << bname << ", 계좌번호: " << anum << ", 비밀번호: " << pw << ", 계좌보유금: " << ac << ", 카드 번호: " << cn << ", 사용자 이름:" << un << endl;
}; // 계좌 초기 설정

class Multi_ATM {
protected:
    Bank* main_bank; // 주 은행 설정
    int ATM_serial; // 고유 넘버 6자리
    string lang_type; // 단일 언어, 이중 언어(한영)
    int cash_1000; // ATM 1000원짜리 현금
    int cash_5000; // ATM 5000원짜리 현금
    int cash_10000; // ATM 10000원짜리 현금
    int cash_50000; // ATM 50000원짜리 현금
    int p_deposit_fee;
    int n_deposit_fee;
    int p_withdrawal_fee;
    int n_withdrawal_fee;
    int pp_transfer_fee;
    int pn_transfer_fee;
    int nn_transfer_fee;
    int cash_transfer_fee;
    static int withdrawal_num;
    static vector<Multi_ATM*> ATM_list;
    vector<Trans_History> history_lst;
    vector<Trans_History> session_lst;

public:
    Multi_ATM(Bank* mbank, int sernum, string lang, int num_1000, int num_5000, int num_10000, int num_50000, int p_dep, int n_dep, int p_with, int n_with, int pp_trans, int pn_trans, int nn_trans, int cash_trans) {
        main_bank = mbank;
        ATM_serial = sernum;
        lang_type = lang;
        cash_1000 = num_1000;
        cash_5000 = num_5000;
        cash_10000 = num_10000;
        cash_50000 = num_50000;
		p_deposit_fee = p_dep;
		n_deposit_fee = n_dep;
		p_withdrawal_fee = p_with;
		n_withdrawal_fee = n_with;
		pp_transfer_fee = pp_trans;
		pn_transfer_fee = pn_trans;
		nn_transfer_fee = nn_trans;
		cash_transfer_fee = cash_trans;
		static int withdrawal_num;
		ATM_list.push_back(this);
    }; // ATM 초기 설정

	static void addATMToList(Multi_ATM* atm) {
		ATM_list.push_back(atm);
	}

	void set_atmlist(vector<Multi_ATM*> atm) {
		ATM_list = atm;
	}

	vector<Multi_ATM*> get_atmlist() {
		return ATM_list;
	}


    Bank* get_ATMBank() { return main_bank; } // ATM의 primiary bank 반환
    int get_serial() { return ATM_serial; } // ATM의 일련번호 반환

	Multi_ATM* find_atm(int num) {
		vector<Multi_ATM*>::iterator iter_atm;
		for (iter_atm = ATM_list.begin(); iter_atm != ATM_list.end(); ++iter_atm) {
			if ((*iter_atm)->get_serial() == num) {
				return (*iter_atm);
			}
		}
		return nullptr;
    }

    string get_lang_type() { return lang_type; } // ATM의 language type 반환 (Unilingual or Bilingual)
    int get_remaining_cash() { return 50000 * cash_50000 + 10000 * cash_10000 + 5000 * cash_5000 + 1000 * cash_1000; }
	void display_ATM() {
		vector<Multi_ATM*>::iterator iter_ATM;
		vector <Account*>::iterator iter_account;
		for (iter_ATM = ATM_list.begin()+1; iter_ATM != ATM_list.end(); ++iter_ATM) {
            cout << "ATM [SN: " << (*iter_ATM)->get_serial() << "] remaining cash: " << (*iter_ATM)->get_remaining_cash() << endl;
		}
	}

    void start_ATM() {
        if (lang_type == "Uni") {
            lang_type = "1";
        }
        else if (lang_type == "Bi") {
            choicelang();
            atmui_e();
            cin >> lang_type;
        }
        insert_card();
    }
    virtual void insert_card() {
        int card_num;
        hello(lang_type);
        cin >> card_num;
        if (card_num == ATM_serial) {
            admin(lang_type);
            int ans;
            cin >> ans;
            if (ans == 1) {
                print_all_session();
                return;
            }
            else { return; }
        }

        Account* acc = main_bank->find_account(card_num);
        if (acc == nullptr) {
            notcard(lang_type);
            return;
        }
        enter_password(acc);
    }
    void enter_password(Account* acc) { // 비밀번호 입력 화면
        int password;
        int wrong_time = 0;

        while (true) {
            pw(lang_type);
            cin >> password;
            if (password != acc->get_password()) {
                wrong_time++;
                if (wrong_time == 3) {
                    wrongpw(lang_type);
                    return;
                }
            }
            else {
                outnumber(lang_type);
                mainstage(acc);
                break;
            }
        }
    }
    void mainstage(Account* acc) { // Transaction Main
        totalmenu(lang_type);
        int input;
        cin >> input;
        if (input == 1) {
            depositmenu(lang_type);
            deposit(acc);
            return;
        }
        else if (input == 2) {
            if (withdrawal_num >= 3) {
                w3(lang_type);
                mainstage(acc);
                return;
            }
            withdrawal(lang_type);
            withdrawal(acc);
            return;
        }
        else if (input == 3) {
            transfermenu(lang_type);
            transfer(acc);
            return;
        }
        else if (input == 4) {
            withdrawal_num = 0;
            finish(lang_type);
            print_all_history();
            return;
        }
        else {
            invalidnum(lang_type);
            mainstage(acc);
        }
    }
    void deposit(Account* acc) { // 예금 방법 선택
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
            invalidnum(lang_type);
            deposit(acc);
            return;
        }
    };
    void cash_deposit(Account* acc) {
        int num_50000, num_10000, num_5000, num_1000;
        if (main_bank == acc->get_bank()) {
            depwithfee(lang_type, p_deposit_fee);
			cout << "50000: ";
			cin >> num_50000;
			cout << "10000: ";
			cin >> num_10000;
			cout << "5000: ";
			cin >> num_5000;
			cout << "1000: ";
			cin >> num_1000;
			if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
                banknotelimit(lang_type);
				mainstage(acc);
				return;
			}
			cash_50000 += num_50000;
			cash_10000 += num_10000;
			cash_5000 += num_5000;
			cash_1000 += num_1000;
			acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 - p_deposit_fee);
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_deposit", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, 0));
	        mainstage(acc);
			return;
        }
        else {
            depwithfee(lang_type, n_deposit_fee);
			cout << "50000: ";
			cin >> num_50000;
			cout << "10000: ";
			cin >> num_10000;
			cout << "5000: ";
			cin >> num_5000;
			cout << "1000: ";
			cin >> num_1000;
			if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
                banknotelimit(lang_type);
				mainstage(acc);
				return;
			}
			cash_50000 += num_50000;
			cash_10000 += num_10000;
			cash_5000 += num_5000;
			cash_1000 += num_1000;
			acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 - n_deposit_fee);
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_deposit", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, 0));
			mainstage(acc);
			return;
        }
    }
    void check_deposit(Account* acc) {
        int check_cash, check_num;
        if (main_bank == acc->get_bank()) {
            paper(lang_type);
            cout << "The amount of a check: ";
            cin >> check_cash;
            cout << "Check number: ";
            cin >> check_num;
            if (check_num > 30) {
                checklimit(lang_type);
                mainstage(acc);
                return;
            }
			if (p_deposit_fee != 0) {
				int fee_num;
                checkinputfee(lang_type, p_deposit_fee);
				cout << "1000: ";
                cin >> fee_num;
					if (fee_num*1000 == p_deposit_fee) {
						acc->update_cash(check_cash * check_num);
                        history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "check_deposit", check_cash * check_num, 0));
						mainstage(acc);
						return;
					}
					else {
                        invalidfee(lang_type);
                        mainstage(acc);
                        return;
					}
			}
            else {
                acc->update_cash(check_cash * check_num);
                history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "check_deposit", check_cash * check_num, 0));
                mainstage(acc);
                return;
            }
        }
        else {
			cout << "The amount of a check: ";
			cin >> check_cash;
			cout << "Check number: ";
			cin >> check_num;
			if (check_num > 30) {
                checklimit(lang_type);
				mainstage(acc);
				return;
			}
			if (n_deposit_fee != 0) {
				int fee_num;
                checkinputfee(lang_type, n_deposit_fee);
				cout << "1000: ";
				cin >> fee_num;
				if (fee_num * 1000 == n_deposit_fee) {
					acc->update_cash(check_cash * check_num);
                    history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "check_deposit", check_cash * check_num, 0));
					mainstage(acc);
					return;
				}
				else {
                    invalidfee(lang_type);
					mainstage(acc);
					return;
				}
			}
            else{
                acc->update_cash(check_cash * check_num);
                history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "check_deposit", check_cash * check_num, 0));
                mainstage(acc);
                return;
            }
		}
    }
    void withdrawal(Account* acc) { // 출금
        int num_50000, num_10000, num_5000, num_1000;
        cout << "50000: ";
        cin >> num_50000;
        if (num_50000 > cash_50000) {
            atmnomoney(lang_type);
            mainstage(acc);
            return;
        }
        cout << "10000: ";
        cin >> num_10000;
        if (num_10000 > cash_10000) {
            atmnomoney(lang_type);
            mainstage(acc);
            return;
        }
        cout << "5000: ";
        cin >> num_5000;
        if (num_5000 > cash_5000) {
            atmnomoney(lang_type);
            mainstage(acc);
            return;
        }
        cout << "1000: ";
        cin >> num_1000;
        if (num_1000 > cash_1000) {
            atmnomoney(lang_type);
            mainstage(acc);
            return;
        }
        if (main_bank == acc->get_bank()) {
			if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 > 500000) {
                withdrawlimit(lang_type);
				mainstage(acc);
				return;
			}
            if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + p_withdrawal_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            cout << 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 << " Won withdrawed" << endl;
            cash_50000 -= num_50000;
            cash_10000 -= num_10000;
            cash_5000 -= num_5000;
            cash_1000 -= num_1000;
            acc->update_cash(-(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + p_withdrawal_fee));
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "withdrawal", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, 0));
            withdrawal_num++;
            mainstage(acc);
            return;
        }
        else {
			if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 > 500000) {
                withdrawlimit(lang_type);
				mainstage(acc);
				return;
			}
            if (50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + n_withdrawal_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            cout << 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 << " Won withdrawed" << endl;
            cash_50000 -= num_50000;
            cash_10000 -= num_10000;
            cash_5000 -= num_5000;
            cash_1000 -= num_1000;
            acc->update_cash(-(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 + n_withdrawal_fee));
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "withdrawal", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, 0));
            withdrawal_num++;
            mainstage(acc);
            return;
        }
    };
    void transfer(Account* acc) { // 송금
        int transfer_type;
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
            invalidnum(lang_type);
            transfer(acc);
            return;
        }
    };
    void cash_transfer(Account* acc) {
        int trans_acc_num;
        transferaccount(lang_type);
        cin >> trans_acc_num;
        Account* trans_acc = main_bank->find_trans_account(trans_acc_num);
        if (trans_acc == nullptr) {
            inv_accnum(lang_type);
            mainstage(acc);
            return;
        }
        int num_50000, num_10000, num_5000, num_1000;
        depwithfee(lang_type, cash_transfer_fee);
        cout << "50000: ";
        cin >> num_50000;
        cout << "10000: ";
        cin >> num_10000;
        cout << "5000: ";
        cin >> num_5000;
        cout << "1000: ";
        cin >> num_1000;
        if (num_50000 + num_10000 + num_5000 + num_1000 > 50) {
            banknotelimit(lang_type);
            mainstage(acc);
            return;
        }
        cash_50000 += num_50000;
        cash_10000 += num_10000;
        cash_5000 += num_5000;
        cash_1000 += num_1000;
        acc->update_cash(50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000 - cash_transfer_fee);
        history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "cash_transfer", 50000 * num_50000 + 10000 * num_10000 + 5000 * num_5000 + 1000 * num_1000, trans_acc_num));
        mainstage(acc);
        return;
    }
    void acc_transfer(Account* acc) {
        int trans_acc_num;
        transferaccount(lang_type);
        cin >> trans_acc_num;
        Account* trans_acc = main_bank->find_trans_account(trans_acc_num);
        if (trans_acc == nullptr) {
            inv_accnum(lang_type);
            mainstage(acc);
            return;
        }
        int fund;
        transfermoney(lang_type);
        cin >> fund;
        if (acc->get_bank() == main_bank && trans_acc->get_bank() == main_bank) {
            if (fund + pp_transfer_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + pp_transfer_fee));
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "acc_transfer", fund, trans_acc_num));
            trans_acc->update_cash(fund);
            mainstage(acc);
        }
        else if (acc->get_bank() != main_bank && trans_acc->get_bank() != main_bank) {
            if (fund + nn_transfer_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + nn_transfer_fee));
            trans_acc->update_cash(fund);
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "acc_transfer", fund, trans_acc_num));
            mainstage(acc);
            return;
        }
        else {
            if (fund + pn_transfer_fee > acc->get_cash()) {
                acc_nomoney(lang_type);
                mainstage(acc);
                return;
            }
            acc->update_cash(-(fund + pn_transfer_fee));
            trans_acc->update_cash(fund);
            history_lst.push_back(Trans_History(acc->get_card_num(), acc->get_accountnum(), "acc_transfer", fund, trans_acc_num));
            mainstage(acc);
            return;
        }
    }
    void print_history(Trans_History th) {
        cout << th.get_trans_id() << endl;
        cout << th.get_cardnumber() << endl;
        cout << th.get_accountnumber() << endl;
        cout << th.get_trans_type() << endl;
        cout << th.get_amount() << endl;
        if (th.get_other_acc() != 0) {
            cout << th.get_other_acc() << endl;
        }
        cout << "\n";
    }
    void print_all_history() {
        for (int i = 0; i < history_lst.size(); i++) {
            print_history(history_lst[i]);
            session_lst.push_back(history_lst[i]);
        }
        history_lst.clear();
    }
    void print_all_session() {
        std::ofstream outputFile("output.txt");
        for (int i = 0; i < session_lst.size(); i++) {
            outputFile << session_lst[i].get_trans_id() << endl;
            outputFile << session_lst[i].get_cardnumber() << endl;
            outputFile << session_lst[i].get_accountnumber() << endl;
            outputFile << session_lst[i].get_trans_type() << endl;
            outputFile << session_lst[i].get_amount() << endl;
            if (session_lst[i].get_other_acc() != 0) {
                outputFile << session_lst[i].get_other_acc() << endl;
            }
            outputFile << "\n";
            
        }
        outputFile.close();
    }
    void choicelang() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      Choose your language                                             I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                1. English              2. 한국어                                      I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void atmui_e() {
        cout << "I                                                                                                                 I" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I      ----------------------------              ------------------------------------------------------------     I" << endl;
        cout << "I      I                          I              I                                                          I     I" << endl;
        cout << "I      I  -----   -----   -----   I              I              Cash/Check Insert and Dispenser             I     I" << endl;
        cout << "I      I  I 1 I   I 2 I   I 3 I   I              I                                                          I     I" << endl;
        cout << "I      I  -----   -----   -----   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I                          I                                                                               I" << endl;
        cout << "I      I  -----   -----   -----   I                                                                               I" << endl;
        cout << "I      I  I 4 I   I 5 I   I 6 I   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                                                          I     I" << endl;
        cout << "I      I                          I              I                       Card Insert                        I     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                                                          I     I" << endl;
        cout << "I      I  I 7 I   I 8 I   I 9 I   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  -----   -----   -----   I                                                                               I" << endl;
        cout << "I      I                          I                                                                               I" << endl;
        cout << "I      I  -----                   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  I 0 I     OK    Cancel  I              I                                                          I     I" << endl;
        cout << "I      I  -----                   I              I                      Receipt Print                       I     I" << endl;
        cout << "I      I                          I              I                                                          I     I" << endl;
        cout << "I      ----------------------------              ------------------------------------------------------------     I" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
    }
    void atmui_k() {
        cout << "I                                                                                                                 I" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I      ----------------------------              ------------------------------------------------------------     I" << endl;
        cout << "I      I                          I              I                                                          I     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                    현금/수표 입출금구                    I     I" << endl;
        cout << "I      I  I 1 I   I 2 I   I 3 I   I              I                                                          I     I" << endl;
        cout << "I      I  -----   -----   -----   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I                          I                                                                               I" << endl;
        cout << "I      I  -----   -----   -----   I                                                                               I" << endl;
        cout << "I      I  I 4 I   I 5 I   I 6 I   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                                                          I     I" << endl;
        cout << "I      I                          I              I                       카드 투입구                        I     I" << endl;
        cout << "I      I  -----   -----   -----   I              I                                                          I     I" << endl;
        cout << "I      I  I 7 I   I 8 I   I 9 I   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  -----   -----   -----   I                                                                               I" << endl;
        cout << "I      I                          I                                                                               I" << endl;
        cout << "I      I  -----                   I              ------------------------------------------------------------     I" << endl;
        cout << "I      I  I 0 I    확인   취소    I              I                                                          I     I" << endl;
        cout << "I      I  -----                   I              I                      영수증 발행구                       I     I" << endl;
        cout << "I      I                          I              I                                                          I     I" << endl;
        cout << "I      ----------------------------              ------------------------------------------------------------     I" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
    }
    void hello(string lang) {
        if (lang == "1") {
            hello_e();
            atmui_e();
        }
        else if (lang == "2") {
            hello_k();
            atmui_k();
        }
        else { cout << "1 or 2 !!!"; }
    }

    void hello_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            반갑습니다.                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  거래를 시작하기 위해 카드를 넣어주세요.                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void hello_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                Welocme                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  To start, please insert your debit card                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void notcard(string lang) {
        if (lang == "1") {
            notcard_e();
            atmui_e();
        }
        else if (lang == "2") {
            notcard_k();
            atmui_k();
        }
    }
    void pw_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                   Write down your password.                                           I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void pw_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      비밀번호를 입력하세요.                                           I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void notcard_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        Invalid cardinput                                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void notcard_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       호환되지 않는 카드입니다                                        I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void pw(string lang) {
        if (lang == "1") {
            pw_e();
            atmui_e();
        }
        else if (lang == "2") {
            pw_k();
            atmui_k();
        }
    }
    void wrongpw(string lang) {
        if (lang == "1") {
            wrongpw_e();
            atmui_e();
        }
        else if (lang == "2") {
            wrongpw_k();
            atmui_k();
        }
    }
    void outnumber_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      Exceed number of attempts.                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }

    void outnumber_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                비밀번호 입력 횟수가 초과되었습니다.                                   I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }

    void wrongpw_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        Invalid Password                                               I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                          Retry please.                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }

    void wrongpw_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                     잘못된 비밀번호 입니다.                                           I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        다시 입력해 주세요.                                            I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void outnumber(string lang) {
        if (lang == "1") {
            outnumber_e();
            atmui_e();
        }
        else if (lang == "2") {
            outnumber_k();
            atmui_k();
        }
    }

    void depositmenu_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                         Choose what you want                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  1) Cash  2) Check  3) Quit  4) Menu                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }


    void depositmenu_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      원하시는 형식을 선택해주세요                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  1) 현금  2) 수표  3) 나가기  4) 메뉴                                 I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void depositmenu(string lang) {
        if (lang == "1") {
            depositmenu_e();
            atmui_e();
        }
        else if (lang == "2") {
            depositmenu_k();
            atmui_k();
        }
    }
    void totalmenu_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                    Please select transaction                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                    1) Deposit  2) Withdrawal  3) Transfer  4) Account fund  5) Quit                   I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }

    void totalmenu_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      원하시는 거래를 선택해주세요                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                           1) 입금  2) 출금  3) 송금  4) 계좌 금액  5) 나가기                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void totalmenu(string lang) {
        if (lang == "1") {
            totalmenu_e();
            atmui_e();
        }
        else if (lang == "2") {
            totalmenu_k();
            atmui_k();
        }
    }
    void withdrawal_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                   Please input withdrawal money                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void withdrawal_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      출금할 금액을 입력해주세요                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void withdrawal(string lang) {
        if (lang == "1") {
            withdrawal_e();
            atmui_e();
        }
        else if (lang == "2") {
            withdrawal_k();
            atmui_k();
        }
    }
    void w3_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                            출금 횟수가 제한을 넘어섰습니다.(최대 3회)                                 I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void w3_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Out number withdrawal. (Max 3)                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void w3(string lang) {
        if (lang == "1") {
            w3_e();
            atmui_e();
        }
        else if (lang == "2") {
            w3_k();
            atmui_k();
        }
    }
    void transfermenu_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        Choose what you want                                           I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                      1) Cash transfer  2) Account transfer  3) Menu  4) Quit                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transfermenu_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            다음 중 골라주세요                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                               1) 현금 송금  2) 계좌 송금  3) 메뉴  4) 나가기                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transfermenu(string lang) {
        if (lang == "1") {
            transfermenu_e();
            atmui_e();
        }
        else if (lang == "2") {
            transfermenu_k();
            atmui_k();
        }
    }
    void finish_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            Thank you                                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void finish_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            감사합니다.                                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void finish(string lang) {
        if (lang == "1") {
            finish_e();
            atmui_e();
        }
        else if (lang == "2") {
            finish_k();
            atmui_k();
        }
    }
    void admin_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                     Admin Card가 들어왔습니다. 전체 거래 내역을 확인하겠습니까?                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                     1. 네           2. 아니오                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void admin_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                       Admin Card is in. Do you want check All transaction                             I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                     1. Yes                2. No                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void admin(string lang) {
        if (lang == "1") {
            admin_e();
            atmui_e();
        }
        else if (lang == "2") {
            admin_k();
            atmui_k();
        }
    }
    void invalidnum_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Invalid input number                                            I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void invalidnum_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       잘못된 입력입니다.                                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void invalidnum(string lang) {
        if (lang == "1") {
            invalidnum_e();
            atmui_e();
        }
        else if (lang == "2") {
            invalidnum_k();
            atmui_k();
        }
    }
    void atmnomoney_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      There is not enough cash in the ATM                              I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void atmnomoney_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        ATM 보유 현금이 부족합니다                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void atmnomoney(string lang) {
        if (lang == "1") {
            atmnomoney_e();
            atmui_e();
        }
        else if (lang == "2") {
            atmnomoney_k();
            atmui_k();
        }
    }
    void acc_nomoney_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Insufficient funds in account                                   I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void acc_nomoney_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       계좌에 잔액이 부족합니다                                        I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void acc_nomoney(string lang) {
        if (lang == "1") {
            acc_nomoney_e();
            atmui_e();
        }
        else if (lang == "2") {
            acc_nomoney_k();
            atmui_k();
        }
    }
    void banknotelimit_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Banknote limit exeeded                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void banknotelimit_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       지폐 한도 초과                                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void banknotelimit(string lang) {
        if (lang == "1") {
            banknotelimit_e();
            atmui_e();
        }
        else if (lang == "2") {
            banknotelimit_k();
            atmui_k();
        }
    }
    void depwithfee_e(int fee) {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                              Insert cash including fee " << fee << " Won                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void depwithfee_k(int fee) {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                수수료 " << fee << " 원을 포함하여 입금해주십시오                               I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void depwithfee(string lang, int fee) {
        if (lang == "1") {
            depwithfee_e(fee);
            atmui_e();
        }
        else if (lang == "2") {
            depwithfee_k(fee);
            atmui_k();
        }
    }
    void paper_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                              Insert Check                                             I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void paper_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                             수표를 투입해주세요                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void paper(string lang) {
        if (lang == "1") {
            paper_e();
            atmui_e();
        }
        else if (lang == "2") {
            paper_k();
            atmui_k();
        }
    }
    void invalidfee_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       Invalid fee inserted                                            I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void invalidfee_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                       잘못된 수수료가 입력되었습니다                                  I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void invalidfee(string lang) {
        if (lang == "1") {
            invalidfee_e();
            atmui_e();
        }
        else if (lang == "2") {
            invalidfee_k();
            atmui_k();
        }
    }
    void checklimit_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                          Check limit exeeded                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void checklimit_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                          수표 개수 초과                                               I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void checklimit(string lang) {
        if (lang == "1") {
            checklimit_e();
            atmui_e();
        }
        else if (lang == "2") {
            checklimit_k();
            atmui_k();
        }
    }
    void checkinputfee_e(int fee) {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                  Insert deposit fee " << fee << " Won                                I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void checkinputfee_k(int fee) {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                      수수료 " << fee << " 원을 입금해주세요                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void checkinputfee(string lang, int fee) {
        if (lang == "1") {
            checkinputfee_e(fee);
            atmui_e();
        }
        else if (lang == "2") {
            checkinputfee_k(fee);
            atmui_k();
        }
    }
    void withdrawlimit_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                          Withdrawal limit exeeded                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void withdrawlimit_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            인출 한도 초과                                             I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void withdrawlimit(string lang) {
        if (lang == "1") {
            withdrawlimit_e();
            atmui_e();
        }
        else if (lang == "2") {
            withdrawlimit_k();
            atmui_k();
        }
    }
    void transferaccount_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                            Input Account Number                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transferaccount_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                   수령인 계좌 번호를 입력해주세요                                     I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transferaccount(string lang) {
        if (lang == "1") {
            transferaccount_e();
            atmui_e();
        }
        else if (lang == "2") {
            transferaccount_k();
            atmui_k();
        }
    }
    void transfermoney_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                    Please input transfermoney                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transfermoney_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                   송금할 금액을 입력해주세요                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void transfermoney(string lang) {
        if (lang == "1") {
            transfermoney_e();
            atmui_e();
        }
        else if (lang == "2") {
            transfermoney_k();
            atmui_k();
        }
    }
    void inv_accnum_e() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        Invalid account number                                         I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void inv_accnum_k() {
        cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "I                                                                                                                 I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                        잘못된 계좌번호입니다                                          I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    I                                                                                                       I    I" << endl;
        cout << "I    ---------------------------------------------------------------------------------------------------------    I" << endl;
    }
    void inv_accnum(string lang) {
        if (lang == "1") {
            inv_accnum_e();
            atmui_e();
        }
        else if (lang == "2") {
            inv_accnum_k();
            atmui_k();
        }
    }
};

class Single_ATM : public Multi_ATM {
public:
	Single_ATM(Bank* mbank, int sernum, string lang, int num_1000, int num_5000, int num_10000, int num_50000, int p_dep, int n_dep, int p_with, int n_with, int pp_trans, int pn_trans, int nn_trans, int cash_trans) : Multi_ATM( mbank, sernum, lang, num_1000, num_5000, num_10000, num_50000, p_dep, n_dep, p_with, n_with, pp_trans, pn_trans, nn_trans, cash_trans) {
	
    }
    void insert_card() {
        int card_num;
        hello(lang_type);
        cin >> card_num;
        Account* acc = main_bank->find_account(card_num);
        if (acc == nullptr) {
            notcard(lang_type);
            return;
        }
        if (acc->get_bank() != main_bank) {
            notcard(lang_type);
            return;
        }
        enter_password(acc);
    }
};

int Multi_ATM::withdrawal_num = 0;

int Trans_History::id_counter = 0;

vector<Bank*> Bank::bank_list;

vector<Multi_ATM*> Multi_ATM::ATM_list;

int main() {
	Bank* b0 = new Bank("None");
	Multi_ATM* a0 = new Multi_ATM(nullptr, 0, "", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	//Multi_ATM(Bank* mbank, int sernum, string lang, int num_1000, int num_5000, int num_10000, int num_50000, int p_dep, int n_dep, int p_with, int n_with, int pp_trans, int pn_trans, int nn_trans, int cash_trans)

    while (1) {
        int ia;
        cout << "0. Bank 생성 1. Account 생성  2. ATM 생성  3. 거래 시작" << endl;
        cin >> ia;
		if (ia == 0) {
            string bank_name;
			// Bank(string bname)
            cout << "은행 이름 설정" << endl;
            cin >> bank_name;
            Bank* b1 = new Bank(bank_name);
            b0->addBankToList(b1);
		}
        else if (ia == 1) {
			// Account(Bank* bname, int anum, int pw, int ac, int cn)
            string abank_name;
            cout << "주 은행 이름을 설정해주세요" << endl;
            cin >> abank_name; 
            //    static vector<Bank*> bank_list;
            Bank* bank = b0->find_bank(abank_name);
            int b;
            int c;
            int d;
            int e;
            string f;
            cout << "계좌번호를 설정해주세요" << endl;
            cin >> b;
            cout << "비밀번호를 설정해주세요" << endl;
            cin >> c;
            cout << "계좌 잔액을 설정해주세요" << endl;
            cin >> d;
            cout << "계좌 연동 카드 번호를 설정해주세요" << endl;
            cin >> e;
            cout << "사용자 이름을 입력해주세요" << endl;
            cin >> f;
            Account::Account(bank, b, c, d, e, f);
            //for (Bank* bank : Bank::bank_list){
        } 
        else if (ia == 2){
            int aa;
            cout << "1. 멀티 지원 ATM   2. 단일 지원 ATM";
            cin >> aa;
			//ATM(Bank* mbank, int sernum, string type, string lang, int num_1000, int num_5000, int num_10000, int num_50000, int p_dep, int n_dep, int p_with, int n_with, int pp_trans, int pn_trans, int nn_trans, int cash_trans)
            if (aa == 1) {
                //Multi_ATM(Bank* mbank, int sernum, string lang, int num_1000, int num_5000, int num_10000, int num_50000, int p_dep, int n_dep, int p_with, int n_with, int pp_trans, int pn_trans, int nn_trans, int cash_trans)
                string Abank_name;
                int sn;
                string lg ; //Uni, Bi 인지
                int n1; // 1000
                int n5; // 5000
                int n10; // 10000
                int n50; // 50000
                int pdf;
                int ndf;
                int pwf;
                int nwf;
                int pptf;
                int pntf;
                int nntf;
                int ctf;
                cout << "주 은행 이름을 설정해주세요" << endl;
                cin >> Abank_name; 
                Bank* bank = b0->find_bank(Abank_name);
                cout << "ATM 고유 넘버를 설정해주세요" << endl;
                cin >> sn;
                cout << "ATM 언어지원 형식(Uni-단일 or Bi-복수)(영어로 입력)" << endl;
                cin >> lg;
                cout << "ATM 보유 1000원 갯수" << endl;
                cin >> n1;
                cout << "ATM 보유 5000원 갯수" << endl;
                cin >> n5;
                cout << "ATM 보유 10000원 갯수" << endl;
                cin >> n10;
				cout << "ATM 보유 50000원 갯수" << endl;
                cin >> n50;
                cout << "주 은행 입금 수수료" << endl;
                cin >> pdf;
                cout << "보조 은행 입금 수수료" << endl;
                cin >> ndf;
                cout << "주 은행 출금 수수료" << endl;
                cin >> pwf;
				cout << "보조 은행 출금 수수료" << endl;
                cin >> nwf;
				cout << "주 은행에서 주 은행으로 계좌이체 수수료" << endl;
                cin >> pptf;
				cout << "주 은행에서 보조 은행/보조 은행에서 주 은행으로 계좌이체 수수료" << endl;
                cin >> pntf;
				cout << "보조 은행에서 보조 은행으로 계좌이체 수수료" << endl;
                cin >> nntf;
				cout << "현금 송금 수수료" << endl;
                cin >> ctf;
				Multi_ATM* a1 = new Multi_ATM(bank, sn, lg, n1, n5, n10, n50, pdf, ndf, pwf, nwf, pptf, pntf, nntf, ctf);
                a0->addATMToList(a1);
                //멀티 ATM Constructor
            }
            else if (aa == 2){
				//Multi_ATM(Bank* mbank, int sernum, string lang, int num_1000, int num_5000, int num_10000, int num_50000, int p_dep, int n_dep, int p_with, int n_with, int pp_trans, int pn_trans, int nn_trans, int cash_trans)
				string Abank_name;
				int sn;
				string lg; //Uni, Bi 인지
				int n1; // 1000
				int n5; // 5000
				int n10; // 10000
				int n50; // 50000
				int pdf;
				int ndf;
				int pwf;
				int nwf;
				int pptf;
				int pntf;
				int nntf;
				int ctf;
				cout << "주 은행 이름을 설정해주세요" << endl;
				cin >> Abank_name;
				Bank* bank = b0->find_bank(Abank_name);
				cout << "ATM 고유 넘버를 설정해주세요" << endl;
				cin >> sn;
				cout << "ATM 언어지원 형식(Uni-단일 or Bi-복수)(영어로 입력)" << endl;
				cin >> lg;
				cout << "ATM 보유 1000원 갯수" << endl;
				cin >> n1;
				cout << "ATM 보유 5000원 갯수" << endl;
				cin >> n5;
				cout << "ATM 보유 10000원 갯수" << endl;
				cin >> n10;
				cout << "ATM 보유 50000원 갯수" << endl;
				cin >> n50;
				cout << "주 은행 입금 수수료" << endl;
				cin >> pdf;
				cout << "보조 은행 입금 수수료" << endl;
				cin >> ndf;
				cout << "주 은행 출금 수수료" << endl;
				cin >> pwf;
				cout << "보조 은행 출금 수수료" << endl;
				cin >> nwf;
				cout << "주 은행에서 주 은행으로 계좌이체 수수료" << endl;
				cin >> pptf;
				cout << "주 은행에서 보조 은행/보조 은행에서 주 은행으로 계좌이체 수수료" << endl;
				cin >> pntf;
				cout << "보조 은행에서 보조 은행으로 계좌이체 수수료" << endl;
				cin >> nntf;
				cout << "현금 송금 수수료" << endl;
				cin >> ctf;
                //단일 ATM Constructor
				Single_ATM* a1 = new Single_ATM(bank, sn, lg, n1, n5, n10, n50, pdf, ndf, pwf, nwf, pptf, pntf, nntf, ctf);
				a0->addATMToList(a1);
                }
            else if (ia == 3) {
                break;
            }
        }
    }


	vector<Bank*>::iterator iter_bank;
	for (iter_bank = b0->get_banklist().begin(); iter_bank != b0->get_banklist().end(); ++iter_bank) {
		(*iter_bank)->set_banklist(b0->get_banklist());
	}

	vector<Multi_ATM*>::iterator iter_atm;
	for (iter_atm = a0->get_atmlist().begin(); iter_atm != a0->get_atmlist().end(); ++iter_atm) {
		(*iter_atm)->set_atmlist(a0->get_atmlist());
	}


    while (1) {
        int a;
        cout << "1. 원하는 ATM 선택   2. 프로그램 완전 종료   3. 모든 정보 확인" << endl;
        cin >> a;
        if (a == 1) {
            int num;
            cout << "ATM 고유 번호를 입력하세요" << endl;
            cin >> num;
            Multi_ATM* atm = a0->find_atm(num);
            atm->start_ATM();
        }
        else if (a == 2){
            break;
        }
        else if (a == 3){
            a0->display_ATM();
            b0->display_account();
        }
    }
    return 0;
}