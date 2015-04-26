
	controller::DataController d;

	d.AdministratorList["admin"] = Administrator("admin", SHA1("admin"));
	d.BankTellerList["teller1"] = BankTeller("teller1", SHA1("teller1"));
	d.BankTellerList["teller2"] = BankTeller("teller2", SHA1("teller2"));

	d.ForeignExchangeList["USD"] = ForeignExchange(string("美元"), string("USD"), 6.1241);
	d.ForeignExchangeList["EUR"] = ForeignExchange(string("欧元"), string("EUR"), 6.6380);
	d.ForeignExchangeList["JPY"] = ForeignExchange(string("日元"), string("JPY"), 0.051380);
	d.ForeignExchangeList["HKD"] = ForeignExchange(string("港币"), string("HKD"), 0.79021);
	d.ForeignExchangeList["GBP"] = ForeignExchange(string("英镑"), string("GBP"), 9.2366);
	d.ForeignExchangeList["AUD"] = ForeignExchange(string("澳元"), string("AUD"), 4.7740);
	d.ForeignExchangeList["NZD"] = ForeignExchange(string("新西兰元"), string("NZD"), 4.6556);
	d.ForeignExchangeList["SGD"] = ForeignExchange(string("新加坡元"), string("SGD"), 4.5783);
	d.ForeignExchangeList["CAD"] = ForeignExchange(string("加拿大元"), string("CAD"), 5.0450);
	d.ForeignExchangeList["RUB"] = ForeignExchange(string("卢布"), string("RUB"), 0.1206);

	d.DepositList["活期"] = Deposit(string("活期"), 0.35);
	d.DepositList["三个月"] = Deposit(string("三个月"), 2.35, boost::gregorian::months(3));
	d.DepositList["半年"] = Deposit(string("半年"), 2.55, boost::gregorian::months(6));
	d.DepositList["一年"] = Deposit(string("一年"), 2.75, boost::gregorian::months(12));
	d.DepositList["二年"] = Deposit(string("二年"), 3.25, boost::gregorian::months(24));
	d.DepositList["三年"] = Deposit(string("三年"), 3.75, boost::gregorian::months(36));
	d.DepositList["五年"] = Deposit(string("五年"), 4.00, boost::gregorian::months(60));

	ptree pt;
	ptree pt2 = d.GetDepositListPtree(), pt3 = d.GetForeignExchangePtree();
	xml_writer_settings<string> settings('\t', 1);

	auto child = pt2.get_child(DepositListRoot);

	for(auto &i : child){
        pt.add("LogicConfig." + DepositListPath, i.second.get_value<string>());
	}

	auto child2 = pt3.get_child(ForeignExchangeListRoot);
	for (auto &i : child2) {
		pt.add("LogicConfig." + ForeignExchangeListPath, i.second.get_value<string>());
	}


	write_xml(Config::get().GetLogicConfigPath(), pt, std::locale(), settings);
