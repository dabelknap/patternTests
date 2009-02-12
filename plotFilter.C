plotFilter()
{
TH1F *hEta = new TH1F("hEta","hEta",68,-34,34);  
HcalDigis->Draw("eta>>hEta","et>5");
hEta->SetXTitle("eta");
hEta->SetTitle("Hcal Digi Eta");
c1->Print("HcalDigiEta.png");

TH1F *eEta= new TH1F("eEta","eEta",56,-28,28); 
EcalDigis->Draw("eta>>eEta","et>5");
eEta->SetXTitle("eta");
eEta->SetTitle("Ecal Digi Eta");
c1->Print("EcalDigiEta.png");

TH1F *rgnCard= new TH1F("rgnCard","rgnCard",7,-0.5,6.5);
RgnCands->Draw("card>>rgnCard","et>5");
rgnCard->SetXTitle("rgn card");
rgnCard->SetTitle("Rgn card activity");
c1->Print("RgnCards.png");

TH1F *emCard= new TH1F("emCard","emCard",7,-0.5,6.5);
EmCands->Draw("card>>emCard","et>5");
emCard->SetXTitle("em card");
emCard->SetTitle("EM card activity");
c1->Print("EmCards.png");

}
