#include "iostream"
#include "monitor.h"
#include "queue"
#include "thread"
#include "chrono"


#define MAX_BUFFER_SIZE 10
#define PORTION_SIZE 50 //portion weigth in grams
#define MAX_DELAY 10 //maximum delay in seconds

std::chrono::seconds RandomDelay();
//void DisplayStatus();
void ProduceDough();
void ProduceMeat();
void ProduceCheese();
void ProduceCabbage();
void ConsumeMeat();
void ConsumeCheese();
void ConsumeCabbage();

Semaphore fullSlotsDough(0);
Semaphore emptySlotsDough(MAX_BUFFER_SIZE);
//Semaphore //lockDough(1);
Semaphore mutexDough(1);

Semaphore fullSlotsMeat(0);
Semaphore emptySlotsMeat(MAX_BUFFER_SIZE);
Semaphore mutexMeat(1);

Semaphore fullSlotsCheese(0);
Semaphore emptySlotsCheese(MAX_BUFFER_SIZE);
Semaphore mutexCheese(1);

Semaphore fullSlotsCabbage(0);
Semaphore emptySlotsCabbage(MAX_BUFFER_SIZE);
Semaphore mutexCabbage(1);

std::queue<int> bufferDough, bufferMeat, bufferCheese, bufferCabbage;
int d_loop=0, m_loop=0, ch_loop=0, c_loop=0;
int m_cons=0, ch_cons=0, c_cons=0;

int main(){
    std::thread ProducerDough(ProduceDough);
    std::thread ProducerMeat(ProduceMeat);
    std::thread ProducerCheese(ProduceCheese);
    std::thread ProducerCabbage(ProduceCabbage);

    std::thread ConsumerMeat(ConsumeMeat);
    std::thread ConsumerCheese(ConsumeCheese);
    std::thread ConsumerCabbage(ConsumeCabbage);

    ProducerDough.join();
    ProducerMeat.join();
    ProducerCheese.join();
    ProducerCabbage.join();
    ConsumerMeat.join();
    ConsumerCheese.join();
    ConsumerCabbage.join();

    return 0;
};

std::chrono::seconds RandomDelay(){
    auto delay = std::chrono::seconds (rand() % MAX_DELAY + 1);
    return delay;
};

/*void DisplayStatus(){
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "CURRENT STATE" << std::endl;
    std::cout << "Dough portions:"<< bufferDough.size() << "    loop_count:" << d_loop <<std::endl;
    std::cout << "Meat portions:"<< bufferMeat.size() << "  loop_count:" << m_loop << " cons_count:" << m_cons << std::endl;
    std::cout << "Cheese portions:"<< bufferCheese.size() << "  loop_count:" << ch_loop << "    cons_count:" << ch_cons << std::endl;
    std::cout << "Cabbage portions:"<< bufferCabbage.size() << "    loop_count:" << c_loop << " cons_count:" << c_cons << std::endl;
    std::cout << "--------------------------------------" << std::endl;

};*/

void ProduceDough(){
    int portion;
    while(true){
        d_loop++;
        portion = PORTION_SIZE;
        std::this_thread::sleep_for(RandomDelay() % 5);
        emptySlotsDough.p();
        mutexDough.p();

        std::cout<<"=====================================================================" << std::endl;
        std::cout << "Dough portions:"<< bufferDough.size()  <<std::endl;
        bufferDough.push(portion);
        std::cout << "Successfuly addded a portion of dough to buffer." << std::endl;
        std::cout << "Dough portions:"<< bufferDough.size()  <<std::endl;

        //DisplayStatus();
        mutexDough.v();
        fullSlotsDough.v();
    }
};

void ProduceMeat(){
    int portion;
    while(true){
        m_loop++;
        portion = PORTION_SIZE;
        std::this_thread::sleep_for(RandomDelay());
        emptySlotsMeat.p();
        mutexMeat.p();
        std::cout<<"=====================================================================" << std::endl;
        std::cout << "Meat portions:"<< bufferMeat.size()  <<std::endl;
        bufferMeat.push(portion);
        std::cout << "Successfuly addded a portion of meat to buffer." << std::endl;
        std::cout << "Meat portions:"<< bufferMeat.size()  <<std::endl;
        mutexMeat.v();
        //DisplayStatus();
        fullSlotsMeat.v();
    }
};

void ProduceCheese(){
    int portion;
    while(true){
        ch_loop++;
        portion = PORTION_SIZE;
        std::this_thread::sleep_for(RandomDelay());
        emptySlotsCheese.p();
        mutexCheese.v();
        std::cout<<"=====================================================================" << std::endl;
        std::cout << "Cheese portions:"<< bufferCheese.size()  <<std::endl;
        bufferCheese.push(portion);
        std::cout << "Successfuly addded a portion of cheese to buffer." << std::endl;
        std::cout << "Cheese portions:"<< bufferCheese.size()  <<std::endl;
        mutexCheese.p();
        //DisplayStatus();
        fullSlotsCheese.v();
        }
};

void ProduceCabbage(){
    int portion;
    while(true){
        c_loop++;
        portion = PORTION_SIZE;
        std::this_thread::sleep_for(RandomDelay());
        emptySlotsCabbage.p();
        mutexCabbage.p();
        std::cout<<"=====================================================================" << std::endl;
        std::cout << "Cabbage portions:"<< bufferCabbage.size()  <<std::endl;
        bufferCabbage.push(portion);
        std::cout << "Successfuly addded a portion of cabbage to buffer." << std::endl;
        std::cout << "Cabbage portions:"<< bufferCabbage.size()  <<std::endl;
        mutexCabbage.v();
        //DisplayStatus();
        fullSlotsCabbage.v();
    }
};


void ConsumeMeat(){
    while (true){
        m_cons++;
        std::this_thread::sleep_for(RandomDelay());
        fullSlotsDough.p();
        fullSlotsMeat.p();
        //lockDough.p();
        mutexDough.p();
        mutexMeat.p();
        std::cout<<"=====================================================================" << std::endl;
        std::cout << "Dough portions:"<< bufferDough.size()  <<std::endl;
        std::cout << "Meat portions:"<< bufferMeat.size()  <<std::endl;
        bufferDough.pop();
        bufferMeat.pop();
        std::cout<<"A meat dumpling has been eaten." << std::endl;
        std::cout << "Dough portions:"<< bufferDough.size()  <<std::endl;
        std::cout << "Meat portions:"<< bufferMeat.size()  <<std::endl;
        mutexDough.v();
        mutexMeat.v();
        //lockDough.v();
        //DisplayStatus();
        emptySlotsDough.v();
        emptySlotsMeat.v();
    }
};
void ConsumeCheese() {
    while (true) {
        ch_cons++;
        std::this_thread::sleep_for(RandomDelay());
        fullSlotsDough.p();
        fullSlotsCheese.p();
        //lockDough.p();
        mutexDough.p();
        mutexCheese.p();
        std::cout<<"=====================================================================" << std::endl;
        std::cout << "Dough portions:"<< bufferDough.size()  <<std::endl;
        std::cout << "Cheese portions:"<< bufferCheese.size()  <<std::endl;
        bufferDough.pop();
        //lockDough.v();
        bufferCheese.pop();
        std::cout << "A cheese dumpling has been eaten." << std::endl;
        std::cout << "Dough portions:"<< bufferDough.size()  <<std::endl;
        std::cout << "Cheese portions:"<< bufferCheese.size()  <<std::endl;
        mutexDough.v();
        mutexCheese.v();
        //DisplayStatus();
        emptySlotsDough.v();
        emptySlotsCheese.v();
    }
};

void ConsumeCabbage() {
    while (true) {
        c_cons++;
        std::this_thread::sleep_for(RandomDelay());
        fullSlotsDough.p();
        fullSlotsCabbage.p();
        //lockDough.p();
        mutexDough.p();
        mutexCabbage.p();
        std::cout<<"=====================================================================" << std::endl;
        std::cout << "Dough portions:"<< bufferDough.size()  <<std::endl;
        std::cout << "Cabbage portions:"<< bufferCabbage.size()  <<std::endl;
        bufferDough.pop();
        //lockDough.v();
        bufferCabbage.pop();
        std::cout << "A cabbage dumpling has been eaten." << std::endl;
        std::cout << "Dough portions:"<< bufferDough.size()  <<std::endl;
        std::cout << "Cabbage portions:"<< bufferCabbage.size()  <<std::endl;
        mutexDough.v();
        mutexCabbage.v();
        //DisplayStatus();
        emptySlotsDough.v();
        emptySlotsCabbage.v();
    }
};



