#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofNoFill();
	ofEnableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}

	this->noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 500, true, true, true);
	this->word = "A";
	int sample_count = 500;
	vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
	for (int word_index = 0; word_index < word_path.size(); word_index++) {

		vector<ofPolyline> outline = word_path[word_index].getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			for (auto& vertex : vertices) {

				vertex = vertex + glm::vec3(this->font.stringWidth(this->word) * -0.5, this->font.stringHeight(this->word) * 0.5, 0);
			}
			this->font_location_list.insert(this->font_location_list.end(), vertices.begin(), vertices.end());
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 3;
	while (this->log_list.size() < 2500) {

		int font_location_index = ofRandom(this->font_location_list.size());
		vector<glm::vec3> log;
		log.push_back(this->font_location_list[font_location_index]);
		this->log_list.push_back(log);
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		this->life_list.push_back(ofRandom(90));
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto x = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.003, this->noise_seed.x + ofGetFrameNum() * 0.005)), 0, 1, -15, 15);
		auto y = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.003, this->noise_seed.y + ofGetFrameNum() * 0.005)), 0, 1, -15, 15);
		auto z = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.003, this->noise_seed.z + ofGetFrameNum() * 0.005)), 0, 1, -15, 15);
		this->log_list[i].push_back(this->log_list[i].back() + glm::vec3(x, y, z));
		while (this->log_list[i].size() > 90) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);
	ofRotateY(ofGetFrameNum());

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 30) {

			ofSetLineWidth(1);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}