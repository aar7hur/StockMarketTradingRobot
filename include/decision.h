#ifndef DECISION_HEADER_GUARD
#define DECISION_HEADER_GUARD

#define MAX_STOCHASTIC_PURCHASE 20
#define MAX_STOCHASTIC_SALE 80

#include "readcsv.h"
#include "movingAverage.h"

class Decision{
	private:
		bool purchaseAction;
        float userMoney;
        struct userData {
            float stopLoss;
            float trigger;
            float target;
            float qtdStocks;
            std::string message;
        } outputUser;

        struct dataRisk {
			float highDaily;
			float lowDaily;
		};

		struct dataForDecision {
			float closeWeek;
			float average_20;
			float average_50;
			float stochastic_8;
            struct dataRisk;
		} decisionData;

    MovingAverage movingAverage;
    bool isPurchaseAction(void);
    bool isSaleAction(void);
    void managmentRisk(void);

	public:
        float getUserMoney(void);
        void setUserMoney(float);
		void populateData(std::string, std::string);
        void doDecision(void);
        userData getUserData(void);
		Decision();
		~Decision();	
};

#endif