#include "AccountController.hpp"
#include <boost\log\trivial.hpp>
#include <stdexcept>

const double MinimumAmount = 0.01;

namespace controller
{
	void AccountController::UpdateAmount()
	{
		auto now = boost::gregorian::day_clock::local_day();

		for (auto &i : Account.CurrencyAccountList) {
			double interest;
			auto days = now - i.LastUpdateDate;
			if (i.DespoitType.Duration == boost::date_time::not_a_date_time && !days.is_negative()) //活期
				interest = i.DespoitType.IRPerYear / 360 / 100 * days.days() * i.Currency.Amount;
			else if (i.Period.contains(now)) continue; //未到计息日
			else { //已经过了计息日
				if (i.Period.contains(i.LastUpdateDate)) { //逾期存款按活期计息
					if (!days.is_negative()) interest = DemandRate / 360 / 100 * days.days() * i.Currency.Amount;
				}
				else { //未计息
					interest = i.DespoitType.IRPerYear / 12 / 100 * i.DespoitType.Duration.number_of_months().as_number() * i.Currency.Amount;
					if (!days.is_negative()) interest += DemandRate / 360 / 100 * days.days() * i.Currency.Amount;
				}
			}

			if (interest > MinimumAmount) { //大于最小分度才计息
				i.Currency.Amount += interest;

				Record.emplace_back(Account.Number, Account.Name, "利息", model::Currency(i.Currency.Name, i.Currency.Code, interest), "");
			}

			i.LastUpdateDate = now;
		}
	}

	double AccountController::UpdateCurrencyAccount(size_t index, double amount, bool save)
	{
		auto &acc = Account.CurrencyAccountList[index];
		if (save) {
			acc.Currency.Amount += amount;

			Record.emplace_back(Account.Number, Account.Name, "现存", model::Currency(acc.Currency.Name, acc.Currency.Code, amount), "");
		}
		else {
			auto now = boost::gregorian::day_clock::local_day();
			auto days = now - acc.LastUpdateDate;
			if (amount > acc.Currency.Amount) throw std::invalid_argument("Insufficient account balance");
			if (acc.DespoitType.Duration == boost::date_time::not_a_date_time || !acc.Period.contains(now)) { //活期或已逾期的定期存款
				acc.Currency.Amount -= amount;
				Record.emplace_back(Account.Number, Account.Name, "取现", model::Currency(acc.Currency.Name, acc.Currency.Code, -amount), "");
			}
			else { //提前支取
				 
				acc.Currency.Amount -= amount;
				amount += DemandRate / 360 / 100 * days.days() * amount;

				Record.emplace_back(Account.Number, Account.Name, "取现", model::Currency(acc.Currency.Name, acc.Currency.Code, -amount), "定期存款提前支取");
			}
		}

		return amount;
	}

	void AccountController::NewCurrencyAccount(const model::CurrencyAccount &acc)
	{
		Account.CurrencyAccountList.push_back(acc);

		Record.emplace_back(Account.Number, Account.Name, "开户", acc.Currency, "");
	}

	double AccountController::DelCurrencyAccount(size_t index)
	{
		auto acc = Account.CurrencyAccountList[index];
		double amount = acc.Currency.Amount;
		auto now = boost::gregorian::day_clock::local_day();
		
		if (acc.Period.contains(now)) {
			auto days = now - acc.LastUpdateDate;
			amount += DemandRate / 360 / 100 * days.days() * amount;
		}

		Account.CurrencyAccountList.erase(Account.CurrencyAccountList.begin() + index);

		Record.emplace_back(Account.Number, Account.Name, "销户", acc.Currency, "");

		return amount;

	}

	void AccountController::Transfer(std::string target, double amount)
	{
		Account.CurrencyAccountList[0].Currency.Amount -= amount;
		Record.emplace_back(Account.Number, Account.Name, "转账", model::Currency(model::MainCurrency.Name, model::MainCurrency.Code, -1 * (amount)), "目标卡号:" + target);
	}

}
