use std::io;
use std::collections::HashMap;

fn main() {
    let mut currencies = HashMap::new();
    // Converting from one USD to currency.
    currencies.insert(String::from("USD"), 1.00);
    currencies.insert(String::from("EUR"), 0.86);
    currencies.insert(String::from("GBP"), 0.76);
    currencies.insert(String::from("JPY"), 156.32);
    currencies.insert(String::from("AUD"), 1.53);
    currencies.insert(String::from("CAD"), 1.4);
    currencies.insert(String::from("CHF"), 0.8);
    currencies.insert(String::from("CNY"), 7.08);

    println!("Enter an amount of money: ");

    let mut num = String::new();

    io::stdin()
        .read_line(&mut num)
        .expect("Failed to read line!");

    let num: f64 = num.trim().parse::<f64>().expect("Please enter a number!");

    println!("Type a currency code to convert from: ");

    let mut currency_from = String::new();

    io::stdin()
        .read_line(&mut currency_from)
        .expect("Failed to read line!");

    println!("Type a currency code to convert to: ");

    let mut currency_to = String::new();

    io::stdin()
        .read_line(&mut currency_to)
        .expect("Failed to read line!");

    currency_from = currency_from.trim().parse::<String>().unwrap();
    currency_to = currency_to.trim().parse::<String>().unwrap();

    println!("You are converting {num} from {currency_from} to {currency_to}.");
    let usd_conversion = (1.0 / (currencies.get(&currency_from).copied().unwrap())) * num;

    let final_conversion = (((currencies.get(&currency_to).copied().unwrap()) * usd_conversion) * 100.0).round() / 100.0;

    println!("{num} from {currency_from} to {currency_to} is {final_conversion}.");
}
