#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

class Card {
public:
    string suit;
    string rank;
    int value;

    Card(string r, string s, int v) : rank(r), suit(s), value(v) {}
};

class Deck {
private:
    vector<Card> cards;

public:
    Deck() {
        string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
        string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
        int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

        for (const string &suit : suits) {
            for (int i = 0; i < 13; ++i) {
                cards.emplace_back(ranks[i], suit, values[i]);
            }
        }
    }

    void shuffle() {
        srand(static_cast<unsigned int>(time(0)));
        random_shuffle(cards.begin(), cards.end());
    }

    Card draw() {
        Card topCard = cards.back();
        cards.pop_back();
        return topCard;
    }
};

class Player {
public:
    string name;
    vector<Card> hand;

    Player(string n) : name(n) {}

    int getHandValue() const {
        int value = 0, aces = 0;

        for (const Card &card : hand) {
            value += card.value;
            if (card.rank == "Ace") {
                aces++;
            }
        }

        while (value > 21 && aces > 0) {
            value -= 10;
            aces--;
        }

        return value;
    }

    void addCard(const Card &card) {
        hand.push_back(card);
    }

    void showHand(bool hideFirstCard = false) const {
        cout << name << "'s hand: ";
        for (size_t i = 0; i < hand.size(); ++i) {
            if (hideFirstCard && i == 0) {
                cout << "[Hidden Card] ";
            } else {
                cout << hand[i].rank << " of " << hand[i].suit << " ";
            }
        }
        cout << endl;
    }
};

int main() {
    Deck deck;
    deck.shuffle();

    Player player("Player");
    Player dealer("Dealer");

    player.addCard(deck.draw());
    player.addCard(deck.draw());
    dealer.addCard(deck.draw());
    dealer.addCard(deck.draw());

    bool gameOver = false;

    while (!gameOver) {
        player.showHand();
        dealer.showHand(true);

        cout << "Your hand value: " << player.getHandValue() << endl;
        if (player.getHandValue() > 21) {
            cout << "You busted! Dealer wins!" << endl;
            gameOver = true;
            break;
        }

        cout << "Hit or stand? (h/s): ";
        char choice;
        cin >> choice;

        if (choice == 'h') {
            player.addCard(deck.draw());
        } else if (choice == 's') {
            break;
        } else {
            cout << "Invalid input. Please enter 'h' or 's'." << endl;
        }
    }

    if (!gameOver) {
        while (dealer.getHandValue() < 17) {
            dealer.addCard(deck.draw());
        }

        player.showHand();
        dealer.showHand();

        int playerValue = player.getHandValue();
        int dealerValue = dealer.getHandValue();

        cout << "Your hand value: " << playerValue << endl;
        cout << "Dealer's hand value: " << dealerValue << endl;

        if (dealerValue > 21 || playerValue > dealerValue) {
            cout << "You win!" << endl;
        } else if (playerValue < dealerValue) {
            cout << "Dealer wins!" << endl;
        } else {
            cout << "It's a tie!" << endl;
        }
    }

    return 0;
}
