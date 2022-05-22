int waterPerPlant(){
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER WATER ",11);
	lcd_line_two();
	lcd_string("AMOUNT(ml):",11);
	key = scankey();
	//if(key == '*') break;
	int wtrPerPlant = displayKey();
	return wtrPerPlant;
}

int fertilizePerPlant(){
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER FERTILIZER",16);
	lcd_line_two();
	lcd_string("AMOUNT(ml):",11);
	key = scankey();
	//if(key == '*') break;
	int fertilizePerPlant = displayKey();
	return fertilizePerPlant;
}

int humidityMin(){
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER HUMIDITY",16);
	lcd_line_two();
	lcd_string("MIN(%RH):",9);
	key = scankey();
	//if(key == '*') break;
	int humidityMin = displayKey();
	return humidityMin;
}

int humidityMax(){
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER HUMIDITY",16);
	lcd_line_two();
	lcd_string("MAX(%RH):",9);
	key = scankey();
	//if(key == '*') break;
	int humidityMax = displayKey();
	return humidityMax;
}

int tempMin(){
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER TEMPARAT.",15);
	lcd_line_two();
	lcd_string("MIN(C):",7);
	key = scankey();
	//if(key == '*') break;
	int tempMin = displayKey();
	return tempMin;
}

int tempMax(){
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER TEMPARAT.",15);
	lcd_line_two();
	lcd_string("MAX(C):",7);
	key = scankey();
	//if(key == '*') break;
	int tempMax = displayKey();
	return tempMax;
}