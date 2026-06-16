#include "plugin-vcv.hh"

rack::Plugin *pluginInstance;

extern rack::Model *modelClap;
extern rack::Model *modelClave;
extern rack::Model *modelCowbell;
extern rack::Model *modelHiHat;
extern rack::Model *modelKick;
extern rack::Model *modelMaraca;
extern rack::Model *modelRimshot;
extern rack::Model *modelSnare;
extern rack::Model *modelTom;
extern rack::Model *modelCongabongo;
extern rack::Model *modelDuck;
extern rack::Model *modelAccent;

void init(rack::Plugin *p) {
	pluginInstance = p;


	p->addModel(modelClave);
	p->addModel(modelCowbell);
	p->addModel(modelAccent);
	p->addModel(modelClap);
	p->addModel(modelCongabongo);
	p->addModel(modelHiHat);
	p->addModel(modelMaraca);
	p->addModel(modelRimshot);
	p->addModel(modelKick);
	p->addModel(modelSnare);
	p->addModel(modelTom);
	p->addModel(modelDuck);
}
