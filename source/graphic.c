/* This file is part of PKSM

Copyright (C) 2016 Bernardo Giordano

>    This program is free software: you can redistribute it and/or modify
>    it under the terms of the GNU General Public License as published by
>    the Free Software Foundation, either version 3 of the License, or
>    (at your option) any later version.
>
>    This program is distributed in the hope that it will be useful,
>    but WITHOUT ANY WARRANTY; without even the implied warranty of
>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>    GNU General Public License for more details.
>
>    You should have received a copy of the GNU General Public License
>    along with this program.  If not, see <http://www.gnu.org/licenses/>.
>    See LICENSE for information.
*/

#include <sf2d.h>
#include <sftd.h>
#include <sfil.h>
#include <stdio.h>
#include <time.h>
#include "graphic.h"
#include "fx.h"
#include "editor.h"
#include "bank.h"
#include "http.h"
#include "hex.h"

#define DEBUG 0

int lookup[] = {0x0, 0x1, 0x2, 0x4, 0x5, 0x3};

sftd_font *unicodeJPN12, *unicodeKOR12, *fontBold18, *fontBold15, *fontBold14, *fontBold12, *fontBold11, *fontBold9, *fontFixed; 
sf2d_texture *hexIcon, *hexBG, *hexButton, *blueTextBox, *otaButton, *generationBG, *generationButton, *includeInfoButton, *upperTextGS, *lowerTextGS, *hiddenPowerBG, *hiddenPowerButton, *selectBoxButton, *ballsBG, *ballButton, *male, *female, *naturesButton, *naturestx, *movesBottom, *topMovesBG, *editorBar, *editorStatsBG, *subArrow, *backgroundTop, *miniBox, *plusButton, *minusButton, *balls, *typesSheet, *transferButton, *bankTop, *shinyStar, *normalBar, *LButton, *RButton, *creditsTop, *pokeball, *topBorder, *bottomBorder, *gameSelectorBottom, *gameSelectorTop, *mainMenuBottom, *menuBar, *menuSelectedBar, *darkButton, *eventTop, *left, *lightButton, *redButton, *right, *spritesSmall, *eventMenuBottomBar, *eventMenuTopBarSelected, *eventMenuTopBar, *warningTop, *warningBottom, *boxView, *infoView, *selector, *editorBG, *plus, *minus, *back, *setting, *selectorCloning, *button, *bottomPopUp, *pokemonBufferBox, *cleanTop, *DSBottomBG, *DSTopBG, *DSBarSelected, *DSBar, *DSEventBottom, *DSLangSelected, *DSLang, *DSEventTop, *DSNormalBarL, *DSNormalBarR, *DSSelectedBarL, *DSSelectedBarR, *topSelectedMove, *settings, *item, *alternativeSpritesSmall;

wchar_t *gamesList[] = {L"X", L"Y", L"오루", L"알사", L"썬", L"문", L"D", L"P", L"PL", L"HG", L"SS", L"B", L"W", L"B2", L"W2"};

static wchar_t *moves[] = {L"None", L"막치기", L"태권당수", L"연속뺨치기", L"연속펀치", L"메가톤펀치", L"고양이돈받기", L"불꽃펀치", L"냉동펀치", L"번개펀치", L"할퀴기", L"찝기", L"가위자르기", L"칼바람", L"칼춤", L"풀베기", L"바람일으키기", L"날개치기", L"날려버리기", L"공중날기", L"조이기", L"힘껏치기", L"덩굴채찍", L"짓밟기", L"두번치기", L"메가톤킥", L"점프킥", L"돌려차기", L"모래뿌리기", L"박치기", L"뿔찌르기", L"마구찌르기", L"뿔드릴", L"몸통박치기", L"누르기", L"김밥말이", L"돌진", L"난동부리기", L"이판사판태클", L"꼬리흔들기", L"독침", L"더블니들", L"바늘미사일", L"째려보기", L"물기", L"울음소리", L"울부짖기", L"노래하기", L"초음파", L"소닉붐", L"사슬묶기", L"용해액", L"불꽃세례", L"화염방사", L"흰안개", L"물대포", L"하이드로펌프", L"파도타기", L"냉동빔", L"눈보라", L"환상빔", L"거품광선", L"오로라빔", L"파괴광선", L"쪼기", L"회전부리", L"지옥의바퀴", L"안다리걸기", L"카운터", L"지구던지기", L"괴력", L"흡수", L"메가드레인", L"씨뿌리기", L"성장", L"잎날가르기", L"솔라빔", L"독가루", L"저리가루", L"수면가루", L"꽃잎댄스", L"실뿜기", L"용의분노", L"회오리불꽃", L"전기쇼크", L"10만볼트", L"전기자석파", L"번개", L"돌떨구기", L"지진", L"땅가르기", L"구멍파기", L"맹독", L"염동력", L"사이코키네시스", L"최면술", L"요가포즈", L"고속이동", L"전광석화", L"분노", L"순간이동", L"나이트헤드", L"흉내내기", L"싫은소리", L"그림자분신", L"HP회복", L"단단해지기", L"작아지기", L"연막", L"이상한빛", L"껍질에숨기", L"웅크리기", L"배리어", L"빛의장막", L"흑안개", L"리플렉터", L"기충전", L"참기", L"손가락흔들기", L"따라하기", L"자폭", L"알폭탄", L"핥기", L"스모그", L"오물공격", L"뼈다귀치기", L"불대문자", L"폭포오르기", L"껍질끼우기", L"스피드스타", L"로케트박치기", L"가시대포", L"휘감기", L"망각술", L"숟가락휘기", L"알낳기", L"무릎차기", L"뱀눈초리", L"꿈먹기", L"독가스", L"구슬던지기", L"흡혈", L"악마의키스", L"불새", L"변신", L"거품", L"잼잼펀치", L"버섯포자", L"플래시", L"사이코웨이브", L"튀어오르기", L"녹기", L"찝게햄머", L"대폭발", L"마구할퀴기", L"뼈다귀부메랑", L"잠자기", L"스톤샤워", L"필살앞니", L"각지기", L"텍스처", L"트라이어택", L"분노의앞니", L"베어가르기", L"대타출동", L"발버둥", L"스케치", L"트리플킥", L"도둑질", L"거미집", L"마음의눈", L"악몽", L"화염자동차", L"코골기", L"저주", L"바둥바둥", L"텍스처2", L"에어로블라스트", L"목화포자", L"기사회생", L"원한", L"눈싸라기", L"방어", L"마하펀치", L"겁나는얼굴", L"속여때리기", L"천사의키스", L"배북", L"오물폭탄", L"진흙뿌리기", L"대포무노포", L"압정뿌리기", L"전자포", L"꿰뚫어보기", L"길동무", L"멸망의노래", L"얼다바람", L"판별", L"본러쉬", L"록온", L"역린", L"모래바람", L"기가드레인", L"버티기", L"애교부리기", L"구르기", L"칼등치기", L"뽐내기", L"우유마시기", L"스파크", L"연속자르기", L"강철날개", L"검은눈빛", L"헤롱헤롱", L"잠꼬대", L"치료방울", L"은혜갚기", L"프레젠트", L"화풀이", L"신비의부적", L"아픔나누기", L"성스러운불꽃", L"매그니튜드", L"폭발펀치", L"메가폰", L"용의숨결", L"바톤터치", L"앵콜", L"따라가때리기", L"고속스핀", L"달콤한향기", L"아이언테일", L"메탈크로우", L"받아던지기", L"아침햇살", L"광합성", L"달의불빛", L"잠재파워", L"크로스촙", L"회오리", L"비바라기", L"쾌청", L"깨물어부수기", L"미러코트", L"자기암시", L"신속", L"원시의힘", L"섀도볼", L"미래예지", L"바위깨기", L"바다회오리", L"집단구타", L"속이다", L"소란피기", L"비축하기", L"토해내기", L"꿀꺽", L"열풍", L"싸라기눈", L"트집", L"부추기기", L"도깨비불", L"추억의선물", L"객기", L"힘껏펀치", L"정신차리기", L"날따름", L"자연의힘", L"충전", L"도발", L"도우미", L"트릭", L"역할", L"희망사항", L"조수", L"뿌리박기", L"엄청난힘", L"매직코트", L"리사이클", L"리벤지", L"깨트리다", L"하품", L"탁쳐서떨구기", L"죽기살기", L"분화", L"스킬스웹", L"봉인", L"리프레쉬", L"원념", L"가로챔", L"비밀의힘", L"다이빙", L"손바닥치기", L"보호색", L"반딧불", L"라스트버지", L"미스트볼", L"깃털댄스", L"흔들흔들댄스", L"브레이즈킥", L"흙놀이", L"아이스볼", L"바늘팔", L"태만함", L"하이퍼보이스", L"독엄니", L"브레이크크루", L"블러스트번", L"하이드로캐논", L"코멧펀치", L"놀래키기", L"웨더볼", L"아로마테라피", L"거짓울음", L"에어컷터", L"오버히트", L"냄새구별", L"암석봉인", L"은빛바람", L"금속음", L"풀피리", L"간지르기", L"코스믹파워", L"해수스파우팅", L"시그널빔", L"섀도펀치", L"신통력", L"스카이업퍼", L"모래지옥", L"절대영도", L"탁류", L"기관총", L"제비반환", L"고드름침", L"철벽", L"블록", L"멀리짖음", L"드래곤크루", L"하드플랜트", L"벌크업", L"뛰어오르다", L"머드숏", L"포이즌테일", L"탐내다", L"볼트태클", L"메지컬리프", L"물놀이", L"명상", L"리프블레이드", L"용의춤", L"락블레스트", L"전격파", L"물의파동", L"파멸의소원", L"사이코부스트", L"날개쉬기", L"중력", L"미라클아이", L"잠깨움뺨치기", L"암해머", L"자이로볼", L"치유소원", L"소금물", L"자연의은혜", L"페인트", L"쪼아대기", L"순풍", L"경혈찌르기", L"메탈버스트", L"유턴", L"인파이트", L"보복", L"승부굳히기", L"금제", L"내던지기", L"사이코시프트", L"마지막수단", L"회복봉인", L"쥐어짜기", L"파워트릭", L"위액", L"주술", L"선취", L"흉내쟁이", L"파워스웹", L"가드스웹", L"혼내기", L"뒀다쓰기", L"고민씨", L"기습", L"독압정", L"하트스웹", L"아쿠아링", L"전자부유", L"플레어드라이브", L"발경", L"파동탄", L"록커트", L"독찌르기", L"악의파동", L"깜짝베기", L"아쿠아테일", L"씨폭탄", L"에어슬래시", L"시저크로스", L"벌레의야단법석", L"용의파동", L"드래곤다이브", L"파워젬", L"드레인펀치", L"진공파", L"기합구슬", L"에너지볼", L"브레이브버드", L"대지의힘", L"바꿔치기", L"기가임팩트", L"나쁜음모", L"불릿펀치", L"눈사태", L"얼음뭉치", L"섀도크루", L"번개엄니", L"얼음엄니", L"불꽃엄니", L"야습", L"진흙폭탄", L"사이코커터", L"사념의박치기", L"미러숏", L"러스터캐논", L"락클라임", L"안개제거", L"트릭룸", L"용성군", L"방전", L"분연", L"리프스톰", L"파워휩", L"암석포", L"크로스포이즌", L"더스트슈트", L"아이언헤드", L"마그넷봄", L"스톤에지", L"유혹", L"스텔스록", L"풀묶기", L"수다", L"심판의뭉치", L"벌레먹음", L"차지빔", L"우드해머", L"아쿠아제트", L"공격지령", L"방어지령", L"회복지령", L"양날박치기", L"더블어택", L"시간의포효", L"공간절단", L"초승달춤", L"묵사발", L"마그마스톰", L"다크홀", L"시드플레어", L"괴상한바람", L"섀도다이브", L"손톱갈기", L"와이드가드", L"가드셰어", L"파워셰어", L"원더룸", L"사이코쇼크", L"베놈쇼크", L"바디퍼지", L"분노가루", L"텔레키네시스", L"매직룸", L"떨어뜨리기", L"업어후리기", L"불꽃튀기기", L"오물웨이브", L"나비춤", L"헤비봄버", L"싱크로노이즈", L"일렉트릭볼", L"물붓기", L"니트로차지", L"똬리틀기", L"로킥", L"애시드봄", L"속임수", L"심플빔", L"동료만들기", L"당신먼저", L"돌림노래", L"에코보이스", L"야금야금", L"클리어스모그", L"어시스트파워", L"퍼스트가드", L"사이드체인지", L"열탕", L"껍질깨기", L"치유파동", L"병상첨병", L"프리폴", L"기어체인지", L"배대뒤치기", L"불태우기", L"순서미루기", L"애크러뱃", L"미러타입", L"원수갚기", L"목숨걸기", L"기프트패스", L"연옥", L"물의맹세", L"불꽃의맹세", L"풀의맹세", L"볼트체인지", L"벌레의저항", L"땅고르기", L"얼음숨결", L"드래곤테일", L"분발", L"일렉트릭네트", L"와일드볼트", L"드릴라이너", L"더블촙", L"하트스탬프", L"우드호른", L"성스러운칼", L"셸블레이드", L"히트스탬프", L"그래스믹서", L"하드롤러", L"코튼가드", L"나이트버스트", L"사이코브레이크", L"스위프뺨치기", L"폭풍", L"아프로브레이크", L"기어소서", L"화염탄", L"테크노버스터", L"옛노래", L"신비의칼", L"얼다세계", L"뇌격", L"푸른불꽃", L"불꽃춤", L"프리즈볼트", L"콜드플레어", L"바크아웃", L"고드름떨구기", L"V제너레이트", L"크로스플레임", L"크로스썬더", L"플라잉프레스", L"마룻바닥세워막기", L"트림", L"일구기", L"끈적끈적네트", L"마지막일침", L"고스트다이브", L"핼러윈", L"부르짖기", L"플라스마샤워", L"파라볼라차지", L"Forest's Curse", L"꽃보라", L"프리즈드라이", L"차밍보이스", L"막말내뱉기", L"뒤집어엎기", L"드레인키스", L"트릭가드", L"플라워가드", L"그래스필드", L"미스트필드", L"송전", L"치근거리기", L"요정의바람", L"문포스", L"폭음파", L"페어리록", L"King's Shield", L"친해지기", L"비밀이야기", L"다이아스톰", L"스팀버스트", L"다른차원홀", L"물수리검", L"매지컬플레임", L"니들가드", L"아로마미스트", L"괴전파", L"베놈트랩", L"분진", L"지오컨트롤", L"자기장조작", L"해피타임", L"일렉트릭필드", L"매지컬샤인", L"축하", L"손에손잡기", L"초롱초롱눈동자", L"볼부비부비", L"적당히손봐주기", L"엉겨붙기", L"그로우펀치", L"데스윙", L"사우전드애로", L"사우전드웨이브", L"Land's Wrath", L"파멸의빛", L"근원의파동", L"단애의칼", L"화룡점정", L"다른차원러시", L"Breakneck Blitz", L"Breakneck Blitz", L"All-Out Pummeling", L"All-Out Pummeling", L"Supersonic Skystrike", L"Supersonic Skystrike", L"Acid Downpour", L"Acid Downpour", L"Tectonic Rage", L"Tectonic Rage", L"Continental Crush", L"Continental Crush", L"Savage Spin-Out", L"Savage Spin-Out", L"Never-Ending Nightmare", L"Never-Ending Nightmare", L"Corkscrew Crash", L"Corkscrew Crash", L"Inferno Overdrive", L"Inferno Overdrive", L"Hydro Vortex", L"Hydro Vortex", L"Bloom Doom", L"Bloom Doom", L"Gigavolt Havoc", L"Gigavolt Havoc", L"Shattered Psyche", L"Shattered Psyche", L"Subzero Slammer", L"Subzero Slammer", L"Devastating Drake", L"Devastating Drake", L"Black Hole Eclipse", L"Black Hole Eclipse", L"Twinkle Tackle", L"Twinkle Tackle", L"Catastropika", L"모래모으기", L"만나자마자", L"토치카", L"그림자꿰메기", L"DD래리어트", L"물거품아리아", L"아이스해머", L"플라워힐", L"10만마력", L"힘흡수", L"솔라블레이드", L"나뭇잎", L"스포트라이트", L"독실", L"예민해지기", L"어시스트기어", L"지옥찌르기", L"꽃가루경단", L"앵커숏", L"사이코필드", L"덤벼들기", L"불꽃채찍", L"기어오르기", L"불사르기", L"스피드스웹", L"스마트호른", L"정화", L"잠재댄스", L"코어퍼니셔", L"트로피컬킥", L"지휘", L"부리캐논", L"스케일노이즈", L"드래곤해머", L"세차게휘두르기", L"오로라베일", L"Sinister Arrow Raid", L"Malicious Moonsault", L"Oceanic Operetta", L"Guardian of Alola", L"Soul-Stealing 7-Star Strike", L"Stoked Sparksurfer", L"Pulverizing Pancake", L"Extreme Evoboost", L"Genesis Supernova", L"트랩셸", L"플뢰르캐논", L"사이코팽", L"분함의발구르기", L"섀도본", L"엑셀록", L"아쿠아브레이크", L"프리즘레이저", L"섀도스틸", L"메테오드라이브", L"섀도레이", L"눈물그렁그렁", L"찌리리따끔따끔", L"자연의분노", L"멀티어택", L"10,000,000 Volt Thunderbolt", };
static wchar_t *abilities[] = {L"None", L"악취", L"잔비", L"가속", L"전투무장", L"옹골참", L"습기", L"유연", L"모래숨기", L"정전기", L"축전", L"저수", L"둔감", L"날씨부정", L"복안", L"불면", L"변색", L"면역", L"타오르는불꽃", L"인분", L"마이페이스", L"흡반", L"위협", L"그림자밟기", L"까칠한피부", L"불가사의부적", L"부유", L"포자", L"싱크로", L"클리어바디", L"자연회복", L"피뢰침", L"하늘의은총", L"쓱쓱", L"엽록소", L"발광", L"트레이스", L"천하장사", L"독가시", L"정신력", L"마그마의무장", L"수의베일", L"자력", L"방음", L"젖은접시", L"모래날림", L"프레셔", L"두꺼운지방", L"일찍기상", L"불꽃몸", L"도주", L"날카로운눈", L"괴력집게", L"픽업", L"게으름", L"의욕", L"헤롱헤롱바디", L"플러스", L"마이너스", L"기분파", L"점착", L"탈피", L"근성", L"이상한비늘", L"해감액", L"심록", L"맹화", L"급류", L"벌레의알림", L"돌머리", L"가뭄", L"개미지옥", L"의기양양", L"하얀연기", L"순수한힘", L"조가비갑옷", L"에어록", L"갈지자걸음", L"전기엔진", L"투쟁심", L"불굴의마음", L"눈숨기", L"먹보", L"분노의경혈", L"곡예", L"내열", L"단순", L"건조피부", L"다운로드", L"철주먹", L"포이즌힐", L"적응력", L"스킬링크", L"촉촉바디", L"선파워", L"속보", L"노말스킨", L"스나이퍼", L"매직가드", L"노가드", L"시간벌기", L"테크니션", L"리프가드", L"서투름", L"틀깨기", L"대운", L"유폭", L"위험예지", L"예지몽", L"천진", L"색안경", L"필터", L"슬로스타트", L"배짱", L"마중물", L"아이스바디", L"하드록", L"눈퍼뜨리기", L"꿀모으기", L"통찰", L"이판사판", L"멀티타입", L"플라워기프트", L"나이트메어", L"나쁜손버릇", L"우격다짐", L"심술꾸러기", L"긴장감", L"오기", L"무기력", L"저주받은바디", L"치유의마음", L"프렌드가드", L"깨어진갑옷", L"헤비메탈", L"라이트메탈", L"멀티스케일", L"독폭주", L"열폭주", L"수확", L"텔레파시", L"변덕쟁이", L"방진", L"독수", L"재생력", L"부풀린가슴", L"모래헤치기", L"미라클스킨", L"애널라이즈", L"일루전", L"괴짜", L"틈새포착", L"미라", L"자기과신", L"정의의마음", L"주눅", L"매직미러", L"초식", L"짓궂은마음", L"모래의힘", L"철가시", L"달마모드", L"승리의별", L"터보블레이즈", L"테라볼티지", L"아로마베일", L"플라워베일", L"볼주머니", L"변환자재", L"퍼코트", L"매지션", L"방탄", L"승기", L"옹골찬턱", L"프리즈스킨", L"스위트베일", L"배틀스위치", L"질풍날개", L"메가런처", L"풀모피", L"공생", L"단단한발톱", L"페어리스킨", L"미끈미끈", L"스카이스킨", L"부자유친", L"다크오라", L"페어리오라", L"오라브레이크", L"시작의바다", L"끝의대지", L"델타스트림", L"지구력", L"도망태세", L"위기회피", L"꾸덕꾸덕굳기", L"무도한행동", L"리밋실드", L"잠복", L"수포", L"강철술사", L"발끈", L"눈치우기", L"원격", L"촉촉보이스", L"힐링시프트", L"일렉트릭스킨", L"서핑테일", L"어군", L"탈", L"Battle Bond", L"스웜체인지", L"부식", L"절대안깸", L"여왕의위엄", L"내용물분출", L"무희", L"배터리", L"복슬복슬", L"비비드바디", L"소울하트", L"컬리헤어", L"리시버", L"과학의힘", L"비스트부스트", L"AR시스템", L"일렉트릭메이커", L"사이코메이커", L"미스트메이커", L"그래스메이커", L"메탈프로텍트", L"스펙터가드", L"프리즘아머", };
static wchar_t *movesSorted[] = {L"None", L"흡수", L"엑셀록", L"용해액", L"녹기", L"Acid Downpour", L"Acid Downpour", L"애시드봄", L"애크러뱃", L"경혈찌르기", L"제비반환", L"에어로블라스트", L"당신먼저", L"고속이동", L"에어컷터", L"에어슬래시", L"All-Out Pummeling", L"All-Out Pummeling", L"사이드체인지", L"망각술", L"앵커숏", L"원시의힘", L"아쿠아제트", L"아쿠아링", L"아쿠아테일", L"손바닥치기", L"아로마테라피", L"아로마미스트", L"조수", L"승부굳히기", L"놀래키기", L"공격지령", L"헤롱헤롱", L"파동탄", L"오로라빔", L"오로라베일", L"바디퍼지", L"눈사태", L"초롱초롱눈동자", L"토치카", L"구슬던지기", L"배리어", L"바톤터치", L"부리캐논", L"집단구타", L"트림", L"배북", L"기프트패스", L"참기", L"조이기", L"물기", L"Black Hole Eclipse", L"Black Hole Eclipse", L"블러스트번", L"브레이즈킥", L"눈보라", L"블록", L"Bloom Doom", L"Bloom Doom", L"푸른불꽃", L"누르기", L"뇌격", L"뼈다귀치기", L"본러쉬", L"뼈다귀부메랑", L"폭음파", L"뛰어오르다", L"브레이브버드", L"Breakneck Blitz", L"Breakneck Blitz", L"깨트리다", L"소금물", L"세차게휘두르기", L"거품", L"거품광선", L"벌레먹음", L"벌레의야단법석", L"벌크업", L"땅고르기", L"불릿펀치", L"기관총", L"불사르기", L"명상", L"보호색", L"유혹", L"Catastropika", L"축하", L"충전", L"차지빔", L"애교부리기", L"수다", L"야금야금", L"배대뒤치기", L"껍질끼우기", L"스케일노이즈", L"클리어스모그", L"인파이트", L"똬리틀기", L"연속펀치", L"비밀이야기", L"이상한빛", L"염동력", L"휘감기", L"Continental Crush", L"Continental Crush", L"텍스처", L"텍스처2", L"흉내쟁이", L"코어퍼니셔", L"Corkscrew Crash", L"Corkscrew Crash", L"코스믹파워", L"코튼가드", L"목화포자", L"카운터", L"탐내다", L"찝게햄머", L"트릭가드", L"크로스촙", L"크로스포이즌", L"깨물어부수기", L"브레이크크루", L"묵사발", L"저주", L"풀베기", L"악의파동", L"다크홀", L"DD래리어트", L"매지컬샤인", L"방어지령", L"웅크리기", L"안개제거", L"길동무", L"판별", L"Devastating Drake", L"Devastating Drake", L"다이아스톰", L"구멍파기", L"사슬묶기", L"차밍보이스", L"방전", L"다이빙", L"잼잼펀치", L"파멸의소원", L"더블어택", L"두번치기", L"연속뺨치기", L"그림자분신", L"이판사판태클", L"용성군", L"화룡점정", L"용의숨결", L"드래곤크루", L"용의춤", L"드래곤해머", L"용의파동", L"용의분노", L"드래곤다이브", L"드래곤테일", L"드레인펀치", L"드레인키스", L"꿈먹기", L"회전부리", L"드릴라이너", L"더블촙", L"폭발펀치", L"대지의힘", L"지진", L"에코보이스", L"괴전파", L"알폭탄", L"일렉트릭필드", L"송전", L"일렉트릭볼", L"일렉트릭네트", L"금제", L"불꽃세례", L"앵콜", L"죽기살기", L"버티기", L"에너지볼", L"동료만들기", L"분화", L"대폭발", L"신통력", L"Extreme Evoboost", L"신속", L"객기", L"페어리록", L"요정의바람", L"속이다", L"거짓울음", L"칼등치기", L"깃털댄스", L"페인트", L"속여때리기", L"마지막일침", L"불꽃춤", L"목숨걸기", L"불대문자", L"불꽃엄니", L"불꽃채찍", L"불꽃의맹세", L"불꽃펀치", L"회오리불꽃", L"만나자마자", L"땅가르기", L"바둥바둥", L"불꽃튀기기", L"니트로차지", L"화염자동차", L"화염방사", L"플레어드라이브", L"플래시", L"러스터캐논", L"부추기기", L"플뢰르캐논", L"내던지기", L"플라워힐", L"플라워가드", L"공중날기", L"플라잉프레스", L"기합구슬", L"기충전", L"힘껏펀치", L"날따름", L"발경", L"꿰뚫어보기", L"Forest's Curse", L"속임수", L"프리즈볼트", L"프리즈드라이", L"하드플랜트", L"얼음숨결", L"화풀이", L"마구찌르기", L"연속자르기", L"마구할퀴기", L"크로스썬더", L"크로스플레임", L"미래예지", L"위액", L"기어소서", L"어시스트기어", L"Genesis Supernova", L"지오컨트롤", L"기가드레인", L"기가임팩트", L"Gigavolt Havoc", L"Gigavolt Havoc", L"얼다세계", L"뱀눈초리", L"풀묶기", L"풀의맹세", L"풀피리", L"그래스필드", L"중력", L"울음소리", L"성장", L"원념", L"가드셰어", L"가드스웹", L"Guardian of Alola", L"가위자르기", L"더스트슈트", L"바람일으키기", L"자이로볼", L"싸라기눈", L"암해머", L"해피타임", L"단단해지기", L"흑안개", L"아프로브레이크", L"양날박치기", L"박치기", L"치료방울", L"회복봉인", L"회복지령", L"치유파동", L"치유소원", L"하트스탬프", L"하트스웹", L"히트스탬프", L"열풍", L"헤비봄버", L"도우미", L"병상첨병", L"잠재파워", L"10만마력", L"무릎차기", L"적당히손봐주기", L"손에손잡기", L"손톱갈기", L"뿔찌르기", L"뿔드릴", L"우드호른", L"멀리짖음", L"폭풍", L"하이드로캐논", L"하이드로펌프", L"Hydro Vortex", L"Hydro Vortex", L"파괴광선", L"필살앞니", L"하이퍼보이스", L"다른차원러시", L"다른차원홀", L"최면술", L"아이스볼", L"냉동빔", L"콜드플레어", L"얼음엄니", L"아이스해머", L"냉동펀치", L"얼음뭉치", L"고드름떨구기", L"고드름침", L"얼다바람", L"봉인", L"불태우기", L"연옥", L"Inferno Overdrive", L"Inferno Overdrive", L"엉겨붙기", L"뿌리박기", L"지휘", L"플라스마샤워", L"철벽", L"아이언헤드", L"아이언테일", L"심판의뭉치", L"점프킥", L"태권당수", L"숟가락휘기", L"King's Shield", L"탁쳐서떨구기", L"Land's Wrath", L"예민해지기", L"뒀다쓰기", L"분연", L"리프블레이드", L"리프스톰", L"그래스믹서", L"나뭇잎", L"흡혈", L"씨뿌리기", L"째려보기", L"핥기", L"파멸의빛", L"빛의장막", L"아쿠아브레이크", L"록온", L"악마의키스", L"안다리걸기", L"로킥", L"주술", L"초승달춤", L"덤벼들기", L"라스트버지", L"마하펀치", L"매직코트", L"매직룸", L"메지컬리프", L"마그마스톰", L"마그넷봄", L"전자부유", L"자기장조작", L"매그니튜드", L"Malicious Moonsault", L"마룻바닥세워막기", L"선취", L"검은눈빛", L"요가포즈", L"메가드레인", L"메가톤킥", L"메가톤펀치", L"메가폰", L"추억의선물", L"메탈버스트", L"메탈크로우", L"금속음", L"코멧펀치", L"손가락흔들기", L"우유마시기", L"흉내내기", L"마음의눈", L"작아지기", L"미라클아이", L"미러코트", L"따라하기", L"미러숏", L"흰안개", L"미스트볼", L"미스트필드", L"문포스", L"섀도레이", L"달의불빛", L"아침햇살", L"진흙폭탄", L"머드숏", L"흙놀이", L"진흙뿌리기", L"탁류", L"멀티어택", L"매지컬플레임", L"나쁜음모", L"자연의은혜", L"자연의힘", L"자연의분노", L"바늘팔", L"Never-Ending Nightmare", L"Never-Ending Nightmare", L"나이트버스트", L"나이트헤드", L"깜짝베기", L"악몽", L"부르짖기", L"볼부비부비", L"데스윙", L"Oceanic Operetta", L"대포무노포", L"냄새구별", L"괴상한바람", L"근원의파동", L"역린", L"오버히트", L"아픔나누기", L"파라볼라차지", L"막말내뱉기", L"고양이돈받기", L"보복", L"쪼기", L"멸망의노래", L"꽃보라", L"꽃잎댄스", L"고스트다이브", L"바늘미사일", L"친해지기", L"치근거리기", L"쪼아대기", L"독엄니", L"독가스", L"독찌르기", L"독가루", L"독침", L"포이즌테일", L"꽃가루경단", L"막치기", L"분진", L"눈싸라기", L"파워젬", L"파워셰어", L"파워스웹", L"파워트릭", L"기어오르기", L"파워휩", L"그로우펀치", L"단애의칼", L"프레젠트", L"프리즘레이저", L"방어", L"환상빔", L"자기암시", L"사이코키네시스", L"사이코팽", L"사이코필드", L"사이코부스트", L"사이코커터", L"사이코시프트", L"사이코쇼크", L"사이코브레이크", L"사이코웨이브", L"Pulverizing Pancake", L"혼내기", L"정화", L"따라가때리기", L"순서미루기", L"전광석화", L"퍼스트가드", L"나비춤", L"분노", L"분노가루", L"비바라기", L"고속스핀", L"잎날가르기", L"셸블레이드", L"칼바람", L"HP회복", L"리사이클", L"리플렉터", L"미러타입", L"리프레쉬", L"옛노래", L"잠자기", L"원수갚기", L"은혜갚기", L"잠재댄스", L"리벤지", L"기사회생", L"울부짖기", L"시간의포효", L"락블레스트", L"락클라임", L"록커트", L"스톤샤워", L"바위깨기", L"돌떨구기", L"암석봉인", L"암석포", L"역할", L"돌려차기", L"구르기", L"날개쉬기", L"일구기", L"돌림노래", L"성스러운불꽃", L"성스러운칼", L"신비의부적", L"모래뿌리기", L"모래지옥", L"모래바람", L"Savage Spin-Out", L"Savage Spin-Out", L"열탕", L"겁나는얼굴", L"할퀴기", L"싫은소리", L"화염탄", L"비밀의힘", L"신비의칼", L"씨폭탄", L"시드플레어", L"지구던지기", L"자폭", L"섀도볼", L"섀도본", L"섀도크루", L"섀도다이브", L"섀도펀치", L"야습", L"각지기", L"Shattered Psyche", L"Shattered Psyche", L"절대영도", L"껍질깨기", L"트랩셸", L"기어체인지", L"전격파", L"모래모으기", L"시그널빔", L"은빛바람", L"심플빔", L"노래하기", L"Sinister Arrow Raid", L"스케치", L"스킬스웹", L"로케트박치기", L"불새", L"프리폴", L"스카이업퍼", L"태만함", L"힘껏치기", L"베어가르기", L"수면가루", L"잠꼬대", L"오물공격", L"오물폭탄", L"오물웨이브", L"떨어뜨리기", L"스마트호른", L"정신차리기", L"스모그", L"연막", L"바크아웃", L"가로챔", L"코골기", L"물붓기", L"알낳기", L"솔라빔", L"솔라블레이드", L"소닉붐", L"Soul-Stealing 7-Star Strike", L"공간절단", L"스파크", L"물거품아리아", L"섀도스틸", L"스피드스웹", L"거미집", L"가시대포", L"압정뿌리기", L"니들가드", L"그림자꿰메기", L"토해내기", L"원한", L"튀어오르기", L"버섯포자", L"스포트라이트", L"스텔스록", L"스팀버스트", L"하드롤러", L"강철날개", L"끈적끈적네트", L"비축하기", L"Stoked Sparksurfer", L"짓밟기", L"분함의발구르기", L"스톤에지", L"어시스트파워", L"업어후리기", L"괴력", L"힘흡수", L"실뿜기", L"발버둥", L"벌레의저항", L"저리가루", L"지옥의바퀴", L"대타출동", L"Subzero Slammer", L"Subzero Slammer", L"기습", L"쾌청", L"메테오드라이브", L"분노의앞니", L"엄청난힘", L"초음파", L"Supersonic Skystrike", L"Supersonic Skystrike", L"파도타기", L"뽐내기", L"꿀꺽", L"천사의키스", L"달콤한향기", L"스피드스타", L"바꿔치기", L"칼춤", L"싱크로노이즈", L"광합성", L"몸통박치기", L"반딧불", L"스위프뺨치기", L"꼬리흔들기", L"순풍", L"돌진", L"도발", L"눈물그렁그렁", L"테크노버스터", L"Tectonic Rage", L"Tectonic Rage", L"흔들흔들댄스", L"텔레키네시스", L"순간이동", L"도둑질", L"사우전드애로", L"사우전드웨이브", L"난동부리기", L"지옥찌르기", L"번개", L"번개엄니", L"번개펀치", L"전기쇼크", L"전기자석파", L"10만볼트", L"간지르기", L"뒤집어엎기", L"트집", L"맹독", L"독압정", L"독실", L"변신", L"트라이어택", L"트릭", L"트릭룸", L"핼러윈", L"트리플킥", L"트로피컬킥", L"마지막수단", L"더블니들", L"Twinkle Tackle", L"Twinkle Tackle", L"회오리", L"유턴", L"소란피기", L"V제너레이트", L"진공파", L"베놈트랩", L"베놈쇼크", L"찝기", L"덩굴채찍", L"받아던지기", L"볼트체인지", L"볼트태클", L"잠깨움뺨치기", L"물대포", L"물의맹세", L"물의파동", L"물수리검", L"물놀이", L"해수스파우팅", L"폭포오르기", L"웨더볼", L"바다회오리", L"날려버리기", L"와이드가드", L"와일드볼트", L"도깨비불", L"날개치기", L"희망사항", L"껍질에숨기", L"원더룸", L"우드해머", L"분발", L"고민씨", L"김밥말이", L"쥐어짜기", L"시저크로스", L"하품", L"전자포", L"사념의박치기", L"찌리리따끔따끔", L"10,000,000 Volt Thunderbolt", };
static wchar_t *natures[] = {L"노력", L"외로운", L"용감한", L"고집쟁이", L"개구쟁이", L"대담한", L"온순한", L"무사태평", L"장난꾸러기", L"촐랑거리는", L"겁쟁이", L"성급한", L"성실한", L"명랑", L"천진난만", L"조심스런", L"의젓한", L"냉정한", L"수줍음", L"덜렁한", L"차분한", L"얌전한", L"건방진", L"신중한", L"변덕스러운"};
static wchar_t *items[] = {L"None", L"Master Ball", L"Ultra Ball", L"Great Ball", L"Poke Ball", L"Safari Ball", L"Net Ball", L"Dive Ball", L"Nest Ball", L"Repeat Ball", L"Timer Ball", L"Luxury Ball", L"Premier Ball", L"Dusk Ball", L"Heal Ball", L"Quick Ball", L"Cherish Ball", L"Potion", L"Antidote", L"Burn Heal", L"Ice Heal", L"Awakening", L"Paralyze Heal", L"Full Restore", L"Max Potion", L"Hyper Potion", L"Super Potion", L"Full Heal", L"Revive", L"Max Revive", L"Fresh Water", L"Soda Pop", L"Lemonade", L"Moomoo Milk", L"Energy Powder", L"Energy Root", L"Heal Powder", L"Revival Herb", L"Ether", L"Max Ether", L"Elixir", L"Max Elixir", L"Lava Cookie", L"Berry Juice", L"Sacred Ash", L"HP Up", L"Protein", L"Iron", L"Carbos", L"Calcium", L"Rare Candy", L"PP Up", L"Zinc", L"PP Max", L"Old Gateau", L"Guard Spec.", L"Dire Hit", L"X Attack", L"X Defense", L"X Speed", L"X Accuracy", L"X Sp. Atk", L"X Sp. Def", L"Poké Doll", L"Fluffy Tail", L"Blue Flute", L"Yellow Flute", L"Red Flute", L"Black Flute", L"White Flute", L"Shoal Salt", L"Shoal Shell", L"Red Shard", L"Blue Shard", L"Yellow Shard", L"Green Shard", L"Super Repel", L"Max Repel", L"Escape Rope", L"Repel", L"Sun Stone", L"Moon Stone", L"Fire Stone", L"Thunder Stone", L"Water Stone", L"Leaf Stone", L"Tiny Mushroom", L"Big Mushroom", L"Pearl", L"Big Pearl", L"Stardust", L"Star Piece", L"Nugget", L"Heart Scale", L"Honey", L"Growth Mulch", L"Damp Mulch", L"Stable Mulch", L"Gooey Mulch", L"Root Fossil", L"Claw Fossil", L"Helix Fossil", L"Dome Fossil", L"Old Amber", L"Armor Fossil", L"Skull Fossil", L"Rare Bone", L"Shiny Stone", L"Dusk Stone", L"Dawn Stone", L"Oval Stone", L"Odd Keystone", L"Griseous Orb", L"???", L"???", L"???", L"Douse Drive", L"Shock Drive", L"Burn Drive", L"Chill Drive", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"Sweet Heart", L"Adamant Orb", L"Lustrous Orb", L"Greet Mail", L"Favored Mail", L"RSVP Mail", L"Thanks Mail", L"Inquiry Mail", L"Like Mail", L"Reply Mail", L"Bridge Mail S", L"Bridge Mail D", L"Bridge Mail T", L"Bridge Mail V", L"Bridge Mail M", L"Cheri Berry", L"유루 열매", L"Pecha Berry", L"Rawst Berry", L"Aspear Berry", L"Leppa Berry", L"Oran Berry", L"시몬 열매", L"리샘 열매", L"자몽 열매", L"Figy Berry", L"Wiki Berry", L"Mago Berry", L"Aguav Berry", L"Iapapa Berry", L"Razz Berry", L"Bluk Berry", L"Nanab Berry", L"Wepear Berry", L"Pinap Berry", L"Pomeg Berry", L"Kelpsy Berry", L"Qualot Berry", L"Hondew Berry", L"Grepa Berry", L"Tamato Berry", L"Cornn Berry", L"Magost Berry", L"Rabuta Berry", L"Nomel Berry", L"Spelon Berry", L"Pamtre Berry", L"Watmel Berry", L"Durin Berry", L"Belue Berry", L"오카 열매", L"Passho Berry", L"초나 열매", L"Rindo Berry", L"플카 열매", L"Chople Berry", L"Kebia Berry", L"Shuca Berry", L"Coba Berry", L"Payapa Berry", L"Tanga Berry", L"Charti Berry", L"Kasib Berry", L"Haban Berry", L"Colbur Berry", L"Babiri Berry", L"Chilan Berry", L"Liechi Berry", L"Ganlon Berry", L"캄라 열매", L"Petaya Berry", L"Apicot Berry", L"Lansat Berry", L"Starf Berry", L"Enigma Berry", L"Micle Berry", L"Custap Berry", L"Jaboca Berry", L"Rowap Berry", L"반짝가루", L"하양허브", L"Macho Brace", L"Exp. Share", L"선제공격손톱", L"Soothe Bell", L"Mental Herb", L"구애머리띠", L"King's Rock", L"Silver Powder", L"Amulet Coin", L"Cleanse Tag", L"Soul Dew", L"Deep Sea Tooth", L"Deep Sea Scale", L"Smoke Ball", L"Everstone", L"Focus Band", L"Lucky Egg", L"Scope Lens", L"금속코트", L"먹다남은음식", L"Dragon Scale", L"Light Ball", L"Soft Sand", L"Hard Stone", L"Miracle Seed", L"Black Glasses", L"Black Belt", L"Magnet", L"Mystic Water", L"Sharp Beak", L"Poison Barb", L"Never-Melt Ice", L"Spell Tag", L"Twisted Spoon", L"Charcoal", L"Dragon Fang", L"Silk Scarf", L"Up-Grade", L"조개껍질방울", L"Sea Incense", L"Lax Incense", L"Lucky Punch", L"Metal Powder", L"Thick Club", L"Stick", L"Red Scarf", L"Blue Scarf", L"Pink Scarf", L"Green Scarf", L"Yellow Scarf", L"Wide Lens", L"Muscle Band", L"Wise Glasses", L"Expert Belt", L"빛의점토", L"생명의구슬", L"Power Herb", L"맹독구슬", L"화염구슬", L"Quick Powder", L"기합의띠", L"Zoom Lens", L"Metronome", L"Iron Ball", L"Lagging Tail", L"Destiny Knot", L"검은진흙", L"Icy Rock", L"Smooth Rock", L"Heat Rock", L"Damp Rock", L"Grip Claw", L"구애스카프", L"Sticky Barb", L"Power Bracer", L"Power Belt", L"Power Lens", L"Power Band", L"Power Anklet", L"Power Weight", L"Shed Shell", L"Big Root", L"구애안경", L"Flame Plate", L"Splash Plate", L"Zap Plate", L"Meadow Plate", L"Icicle Plate", L"Fist Plate", L"Toxic Plate", L"Earth Plate", L"Sky Plate", L"Mind Plate", L"Insect Plate", L"Stone Plate", L"Spooky Plate", L"Draco Plate", L"Dread Plate", L"Iron Plate", L"Odd Incense", L"Rock Incense", L"Full Incense", L"Wave Incense", L"Rose Incense", L"Luck Incense", L"Pure Incense", L"Protector", L"Electirizer", L"Magmarizer", L"Dubious Disc", L"Reaper Cloth", L"Razor Claw", L"Razor Fang", L"TM01", L"TM02", L"TM03", L"TM04", L"TM05", L"TM06", L"TM07", L"TM08", L"TM09", L"TM10", L"TM11", L"TM12", L"TM13", L"TM14", L"TM15", L"TM16", L"TM17", L"TM18", L"TM19", L"TM20", L"TM21", L"TM22", L"TM23", L"TM24", L"TM25", L"TM26", L"TM27", L"TM28", L"TM29", L"TM30", L"TM31", L"TM32", L"TM33", L"TM34", L"TM35", L"TM36", L"TM37", L"TM38", L"TM39", L"TM40", L"TM41", L"TM42", L"TM43", L"TM44", L"TM45", L"TM46", L"TM47", L"TM48", L"TM49", L"TM50", L"TM51", L"TM52", L"TM53", L"TM54", L"TM55", L"TM56", L"TM57", L"TM58", L"TM59", L"TM60", L"TM61", L"TM62", L"TM63", L"TM64", L"TM65", L"TM66", L"TM67", L"TM68", L"TM69", L"TM70", L"TM71", L"TM72", L"TM73", L"TM74", L"TM75", L"TM76", L"TM77", L"TM78", L"TM79", L"TM80", L"TM81", L"TM82", L"TM83", L"TM84", L"TM85", L"TM86", L"TM87", L"TM88", L"TM89", L"TM90", L"TM91", L"TM92", L"HM01", L"HM02", L"HM03", L"HM04", L"HM05", L"HM06", L"???", L"???", L"Explorer Kit", L"Loot Sack", L"Rule Book", L"Poké Radar", L"Point Card", L"Journal", L"Seal Case", L"Fashion Case", L"Seal Bag", L"Pal Pad", L"Works Key", L"Old Charm", L"Galactic Key", L"Red Chain", L"Town Map", L"Vs. Seeker", L"Coin Case", L"Old Rod", L"Good Rod", L"Super Rod", L"Sprayduck", L"Poffin Case", L"Bike", L"Suite Key", L"Oak's Letter", L"Lunar Wing", L"Member Card", L"Azure Flute", L"S.S. Ticket", L"Contest Pass", L"Magma Stone", L"Parcel", L"Coupon 1", L"Coupon 2", L"Coupon 3", L"Storage Key", L"Secret Potion", L"Vs. Recorder", L"Gracidea", L"Secret Key", L"Apricorn Box", L"Unown Report", L"Berry Pots", L"Dowsing Machine", L"Blue Card", L"Slowpoke Tail", L"Clear Bell", L"Card Key", L"Basement Key", L"Squirt Bottle", L"Red Scale", L"Lost Item", L"Pass", L"Machine Part", L"Silver Wing", L"Rainbow Wing", L"Mystery Egg", L"Red Apricorn", L"Blue Apricorn", L"Yellow Apricorn", L"Green Apricorn", L"Pink Apricorn", L"White Apricorn", L"Black Apricorn", L"Fast Ball", L"Level Ball", L"Lure Ball", L"Heavy Ball", L"Love Ball", L"Friend Ball", L"Moon Ball", L"Sport Ball", L"Park Ball", L"Photo Album", L"GB Sounds", L"Tidal Bell", L"Rage Candy Bar", L"Data Card 01", L"Data Card 02", L"Data Card 03", L"Data Card 04", L"Data Card 05", L"Data Card 06", L"Data Card 07", L"Data Card 08", L"Data Card 09", L"Data Card 10", L"Data Card 11", L"Data Card 12", L"Data Card 13", L"Data Card 14", L"Data Card 15", L"Data Card 16", L"Data Card 17", L"Data Card 18", L"Data Card 19", L"Data Card 20", L"Data Card 21", L"Data Card 22", L"Data Card 23", L"Data Card 24", L"Data Card 25", L"Data Card 26", L"Data Card 27", L"Jade Orb", L"Lock Capsule", L"Red Orb", L"Blue Orb", L"Enigma Stone", L"Prism Scale", L"Eviolite", L"Float Stone", L"울퉁불퉁멧", L"풍선", L"레드카드", L"Ring Target", L"Binding Band", L"Absorb Bulb", L"Cell Battery", L"탈출버튼", L"Fire Gem", L"Water Gem", L"Electric Gem", L"Grass Gem", L"Ice Gem", L"Fighting Gem", L"Poison Gem", L"Ground Gem", L"Flying Gem", L"Psychic Gem", L"Bug Gem", L"Rock Gem", L"Ghost Gem", L"Dragon Gem", L"Dark Gem", L"Steel Gem", L"Normal Gem", L"Health Wing", L"Muscle Wing", L"Resist Wing", L"Genius Wing", L"Clever Wing", L"Swift Wing", L"Pretty Wing", L"Cover Fossil", L"Plume Fossil", L"Liberty Pass", L"Pass Orb", L"Dream Ball", L"Poké Toy", L"Prop Case", L"Dragon Skull", L"Balm Mushroom", L"Big Nugget", L"Pearl String", L"Comet Shard", L"Relic Copper", L"Relic Silver", L"Relic Gold", L"Relic Vase", L"Relic Band", L"Relic Statue", L"Relic Crown", L"Casteliacone", L"Dire Hit 2", L"X Speed 2", L"X Sp. Atk 2", L"X Sp. Def 2", L"X Defense 2", L"X Attack 2", L"X Accuracy 2", L"X Speed 3", L"X Sp. Atk 3", L"X Sp. Def 3", L"X Defense 3", L"X Attack 3", L"X Accuracy 3", L"X Speed 6", L"X Sp. Atk 6", L"X Sp. Def 6", L"X Defense 6", L"X Attack 6", L"X Accuracy 6", L"Ability Urge", L"Item Drop", L"Item Urge", L"Reset Urge", L"Dire Hit 3", L"Light Stone", L"Dark Stone", L"TM93", L"TM94", L"TM95", L"Xtransceiver", L"???", L"Gram 1", L"Gram 2", L"Gram 3", L"Xtransceiver", L"Medal Box", L"DNA Splicers", L"DNA Splicers", L"Permit", L"Oval Charm", L"Shiny Charm", L"Plasma Card", L"Grubby Hanky", L"Colress Machine", L"Dropped Item", L"Dropped Item", L"Reveal Glass", L"약점보험", L"돌격조끼", L"Holo Caster", L"Prof's Letter", L"Roller Skates", L"Pixie Plate", L"Ability Capsule", L"Whipped Dream", L"Sachet", L"Luminous Moss", L"Snowball", L"방진고글", L"Poké Flute", L"Rich Mulch", L"Surprise Mulch", L"Boost Mulch", L"Amaze Mulch", L"팬텀 나이트", L"가디안 나이트", L"전룡 나이트", L"이상해꽃 나이트", L"리자몽 나이트 X", L"거북왕 나이트", L"뮤츠 나이트 X", L"뮤츠 나이트 Y", L"번치코 나이트", L"요가램 나이트", L"헬가 나이트", L"보스로라 나이트", L"다크펫 나이트", L"마기라스 나이트", L"핫삼 나이트", L"쁘사이저 나이트", L"프테라 나이트", L"루카리오 나이트", L"눈설왕 나이트", L"캥카 나이트", L"갸라도스 나이트", L"앱솔 나이트", L"리자몽 나이트 Y", L"후디 나이트", L"헤라크로스 나이트", L"입치트 나이트", L"썬더볼트 나이트", L"한카리아스 나이트", L"Latiasite", L"Latiosite", L"Roseli Berry", L"Kee Berry", L"Maranga Berry", L"Sprinklotad", L"TM96", L"TM97", L"TM98", L"TM99", L"TM100", L"Power Plant Pass", L"Mega Ring", L"Intriguing Stone", L"Common Stone", L"Discount Coupon", L"Elevator Key", L"TMV Pass", L"Honor of Kalos", L"Adventure Rules", L"Strange Souvenir", L"Lens Case", L"Makeup Bag", L"Travel Trunk", L"Lumiose Galette", L"Shalour Sable", L"Jaw Fossil", L"Sail Fossil", L"Looker Ticket", L"Bike", L"Holo Caster", L"Fairy Gem", L"Mega Charm", L"Mega Glove", L"Mach Bike", L"Acro Bike", L"Wailmer Pail", L"Devon Parts", L"Soot Sack", L"Basement Key", L"Pokéblock Kit", L"Letter", L"Eon Ticket", L"Scanner", L"Go-Goggles", L"Meteorite", L"Key to Room 1", L"Key to Room 2", L"Key to Room 4", L"Key to Room 6", L"Storage Key", L"Devon Scope", L"S.S. Ticket", L"HM07", L"Devon Scuba Gear", L"Contest Costume", L"Contest Costume", L"Magma Suit", L"Aqua Suit", L"Pair of Tickets", L"Mega Bracelet", L"Mega Pendant", L"Mega Glasses", L"Mega Anchor", L"Mega Stickpin", L"Mega Tiara", L"Mega Anklet", L"Meteorite", L"Swampertite", L"Sceptilite", L"Sablenite", L"Altarianite", L"Galladite", L"Audinite", L"Metagrossite", L"Sharpedonite", L"Slowbronite", L"Steelixite", L"Pidgeotite", L"Glalitite", L"Diancite", L"Prison Bottle", L"Mega Cuff", L"Cameruptite", L"Lopunnite", L"Salamencite", L"Beedrillite", L"Meteorite", L"Meteorite", L"Key Stone", L"Meteorite Shard", L"Eon Flute", L"Normalium Z", L"Firium Z", L"Waterium Z", L"Electrium Z", L"Grassium Z", L"Icium Z", L"Fightinium Z", L"Poisonium Z", L"Groundium Z", L"Flyinium Z", L"Psychium Z", L"Buginium Z", L"Rockium Z", L"Ghostium Z", L"Dragonium Z", L"Darkinium Z", L"Steelium Z", L"Fairium Z", L"Pikanium Z", L"Bottle Cap", L"Gold Bottle Cap", L"Z-Ring", L"Decidium Z", L"Incinium Z", L"Primarium Z", L"Tapunium Z", L"Marshadium Z", L"Aloraichium Z", L"Snorlium Z", L"Eevium Z", L"Mewnium Z", L"Normalium Z", L"Firium Z", L"Waterium Z", L"Electrium Z", L"Grassium Z", L"Icium Z", L"Fightinium Z", L"Poisonium Z", L"Groundium Z", L"Flyinium Z", L"Psychium Z", L"Buginium Z", L"Rockium Z", L"Ghostium Z", L"Dragonium Z", L"Darkinium Z", L"Steelium Z", L"Fairium Z", L"Pikanium Z", L"Decidium Z", L"Incinium Z", L"Primarium Z", L"Tapunium Z", L"Marshadium Z", L"Aloraichium Z", L"Snorlium Z", L"Eevium Z", L"Mewnium Z", L"Pikashunium Z", L"Pikashunium Z", L"???", L"???", L"???", L"???", L"Forage Bag", L"Fishing Rod", L"Professor's Mask", L"Festival Ticket", L"Sparkling Stone", L"Adrenaline Orb", L"Zygarde Cube", L"???", L"Ice Stone", L"Ride Pager", L"Beast Ball", L"Big Malasada", L"Red Nectar", L"Yellow Nectar", L"Pink Nectar", L"Purple Nectar", L"Sun Flute", L"Moon Flute", L"???", L"Enigmatic Card", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"Terrain Extender", L"Protective Pads", L"Electric Seed", L"Psychic Seed", L"Misty Seed", L"Grassy Seed", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"Fighting Memory", L"Flying Memory", L"Poison Memory", L"Ground Memory", L"Rock Memory", L"Bug Memory", L"Ghost Memory", L"Steel Memory", L"Fire Memory", L"Water Memory", L"Grass Memory", L"Electric Memory", L"Psychic Memory", L"Ice Memory", L"Dragon Memory", L"Dark Memory", L"Fairy Memory", };
static wchar_t *itemsSorted[] = {L"None", L"Ability Capsule", L"Ability Urge", L"눈설왕 나이트", L"앱솔 나이트", L"Absorb Bulb", L"Acro Bike", L"Adamant Orb", L"Adrenaline Orb", L"Adventure Rules", L"프테라 나이트", L"보스로라 나이트", L"Aguav Berry", L"풍선", L"후디 나이트", L"Aloraichium Z", L"Aloraichium Z", L"Altarianite", L"Amaze Mulch", L"전룡 나이트", L"Amulet Coin", L"Antidote", L"Apicot Berry", L"Apricorn Box", L"Aqua Suit", L"Armor Fossil", L"Aspear Berry", L"돌격조끼", L"Audinite", L"Awakening", L"Azure Flute", L"Babiri Berry", L"Balm Mushroom", L"다크펫 나이트", L"Basement Key", L"Basement Key", L"Beast Ball", L"Beedrillite", L"Belue Berry", L"Berry Juice", L"Berry Pots", L"Big Malasada", L"Big Mushroom", L"Big Nugget", L"Big Pearl", L"Big Root", L"Bike", L"Bike", L"Binding Band", L"Black Apricorn", L"Black Belt", L"Black Flute", L"Black Glasses", L"검은진흙", L"거북왕 나이트", L"번치코 나이트", L"Blue Apricorn", L"Blue Card", L"Blue Flute", L"Blue Orb", L"Blue Scarf", L"Blue Shard", L"Bluk Berry", L"Boost Mulch", L"Bottle Cap", L"Bridge Mail D", L"Bridge Mail M", L"Bridge Mail S", L"Bridge Mail T", L"Bridge Mail V", L"반짝가루", L"Bug Gem", L"Bug Memory", L"Buginium Z", L"Buginium Z", L"Burn Drive", L"Burn Heal", L"Calcium", L"Cameruptite", L"Carbos", L"Card Key", L"Casteliacone", L"Cell Battery", L"Charcoal", L"리자몽 나이트 X", L"리자몽 나이트 Y", L"Charti Berry", L"Cheri Berry", L"Cherish Ball", L"유루 열매", L"Chilan Berry", L"Chill Drive", L"구애머리띠", L"구애스카프", L"구애안경", L"Chople Berry", L"Claw Fossil", L"Cleanse Tag", L"Clear Bell", L"Clever Wing", L"Coba Berry", L"Coin Case", L"Colbur Berry", L"Colress Machine", L"Comet Shard", L"Common Stone", L"Contest Costume", L"Contest Costume", L"Contest Pass", L"Cornn Berry", L"Coupon 1", L"Coupon 2", L"Coupon 3", L"Cover Fossil", L"Custap Berry", L"Damp Mulch", L"Damp Rock", L"Dark Gem", L"Dark Memory", L"Dark Stone", L"Darkinium Z", L"Darkinium Z", L"Data Card 01", L"Data Card 02", L"Data Card 03", L"Data Card 04", L"Data Card 05", L"Data Card 06", L"Data Card 07", L"Data Card 08", L"Data Card 09", L"Data Card 10", L"Data Card 11", L"Data Card 12", L"Data Card 13", L"Data Card 14", L"Data Card 15", L"Data Card 16", L"Data Card 17", L"Data Card 18", L"Data Card 19", L"Data Card 20", L"Data Card 21", L"Data Card 22", L"Data Card 23", L"Data Card 24", L"Data Card 25", L"Data Card 26", L"Data Card 27", L"Dawn Stone", L"Decidium Z", L"Decidium Z", L"Deep Sea Scale", L"Deep Sea Tooth", L"Destiny Knot", L"Devon Parts", L"Devon Scope", L"Devon Scuba Gear", L"Diancite", L"Dire Hit", L"Dire Hit 2", L"Dire Hit 3", L"Discount Coupon", L"Dive Ball", L"DNA Splicers", L"DNA Splicers", L"Dome Fossil", L"Douse Drive", L"Dowsing Machine", L"Draco Plate", L"Dragon Fang", L"Dragon Gem", L"Dragon Memory", L"Dragon Scale", L"Dragon Skull", L"Dragonium Z", L"Dragonium Z", L"Dread Plate", L"Dream Ball", L"Dropped Item", L"Dropped Item", L"Dubious Disc", L"Durin Berry", L"Dusk Ball", L"Dusk Stone", L"Earth Plate", L"Eevium Z", L"Eevium Z", L"탈출버튼", L"Electirizer", L"Electric Gem", L"Electric Memory", L"Electric Seed", L"Electrium Z", L"Electrium Z", L"Elevator Key", L"Elixir", L"Energy Powder", L"Energy Root", L"Enigma Berry", L"Enigma Stone", L"Enigmatic Card", L"Eon Flute", L"Eon Ticket", L"Escape Rope", L"Ether", L"Everstone", L"Eviolite", L"Exp. Share", L"Expert Belt", L"Explorer Kit", L"Fairium Z", L"Fairium Z", L"Fairy Gem", L"Fairy Memory", L"Fashion Case", L"Fast Ball", L"Favored Mail", L"Festival Ticket", L"Fighting Gem", L"Fighting Memory", L"Fightinium Z", L"Fightinium Z", L"Figy Berry", L"Fire Gem", L"Fire Memory", L"Fire Stone", L"Firium Z", L"Firium Z", L"Fishing Rod", L"Fist Plate", L"화염구슬", L"Flame Plate", L"Float Stone", L"Fluffy Tail", L"Flying Gem", L"Flying Memory", L"Flyinium Z", L"Flyinium Z", L"Focus Band", L"기합의띠", L"Forage Bag", L"Fresh Water", L"Friend Ball", L"Full Heal", L"Full Incense", L"Full Restore", L"Galactic Key", L"Galladite", L"Ganlon Berry", L"한카리아스 나이트", L"가디안 나이트", L"GB Sounds", L"팬텀 나이트", L"Genius Wing", L"Ghost Gem", L"Ghost Memory", L"Ghostium Z", L"Ghostium Z", L"Glalitite", L"Go-Goggles", L"Gold Bottle Cap", L"Good Rod", L"Gooey Mulch", L"Gracidea", L"Gram 1", L"Gram 2", L"Gram 3", L"Grass Gem", L"Grass Memory", L"Grassium Z", L"Grassium Z", L"Grassy Seed", L"Great Ball", L"Green Apricorn", L"Green Scarf", L"Green Shard", L"Greet Mail", L"Grepa Berry", L"Grip Claw", L"Griseous Orb", L"Ground Gem", L"Ground Memory", L"Groundium Z", L"Groundium Z", L"Growth Mulch", L"Grubby Hanky", L"Guard Spec.", L"갸라도스 나이트", L"Haban Berry", L"Hard Stone", L"Heal Ball", L"Heal Powder", L"Health Wing", L"Heart Scale", L"Heat Rock", L"Heavy Ball", L"Helix Fossil", L"헤라크로스 나이트", L"HM01", L"HM02", L"HM03", L"HM04", L"HM05", L"HM06", L"HM07", L"Holo Caster", L"Holo Caster", L"Hondew Berry", L"Honey", L"Honor of Kalos", L"헬가 나이트", L"HP Up", L"Hyper Potion", L"Iapapa Berry", L"Ice Gem", L"Ice Heal", L"Ice Memory", L"Ice Stone", L"Icicle Plate", L"Icium Z", L"Icium Z", L"Icy Rock", L"Incinium Z", L"Incinium Z", L"Inquiry Mail", L"Insect Plate", L"Intriguing Stone", L"Iron", L"Iron Ball", L"Iron Plate", L"Item Drop", L"Item Urge", L"Jaboca Berry", L"Jade Orb", L"Jaw Fossil", L"Journal", L"캥카 나이트", L"Kasib Berry", L"Kebia Berry", L"Kee Berry", L"Kelpsy Berry", L"Key Stone", L"Key to Room 1", L"Key to Room 2", L"Key to Room 4", L"Key to Room 6", L"King's Rock", L"Lagging Tail", L"Lansat Berry", L"Latiasite", L"Latiosite", L"Lava Cookie", L"Lax Incense", L"Leaf Stone", L"먹다남은음식", L"Lemonade", L"Lens Case", L"Leppa Berry", L"Letter", L"Level Ball", L"Liberty Pass", L"Liechi Berry", L"생명의구슬", L"Light Ball", L"빛의점토", L"Light Stone", L"Like Mail", L"Lock Capsule", L"Looker Ticket", L"Loot Sack", L"Lopunnite", L"Lost Item", L"Love Ball", L"루카리오 나이트", L"Luck Incense", L"Lucky Egg", L"Lucky Punch", L"리샘 열매", L"Luminous Moss", L"Lumiose Galette", L"Lunar Wing", L"Lure Ball", L"Lustrous Orb", L"Luxury Ball", L"Mach Bike", L"Machine Part", L"Macho Brace", L"Magma Stone", L"Magma Suit", L"Magmarizer", L"Magnet", L"Mago Berry", L"Magost Berry", L"Makeup Bag", L"썬더볼트 나이트", L"Maranga Berry", L"Marshadium Z", L"Marshadium Z", L"Master Ball", L"입치트 나이트", L"Max Elixir", L"Max Ether", L"Max Potion", L"Max Repel", L"Max Revive", L"Meadow Plate", L"Medal Box", L"요가램 나이트", L"Mega Anchor", L"Mega Anklet", L"Mega Bracelet", L"Mega Charm", L"Mega Cuff", L"Mega Glasses", L"Mega Glove", L"Mega Pendant", L"Mega Ring", L"Mega Stickpin", L"Mega Tiara", L"Member Card", L"Mental Herb", L"Metagrossite", L"금속코트", L"Metal Powder", L"Meteorite", L"Meteorite", L"Meteorite", L"Meteorite", L"Meteorite Shard", L"Metronome", L"Mewnium Z", L"Mewnium Z", L"뮤츠 나이트 X", L"뮤츠 나이트 Y", L"Micle Berry", L"Mind Plate", L"Miracle Seed", L"Misty Seed", L"Moomoo Milk", L"Moon Ball", L"Moon Flute", L"Moon Stone", L"Muscle Band", L"Muscle Wing", L"Mystery Egg", L"Mystic Water", L"Nanab Berry", L"Nest Ball", L"Net Ball", L"Never-Melt Ice", L"Nomel Berry", L"Normal Gem", L"Normalium Z", L"Normalium Z", L"Nugget", L"Oak's Letter", L"오카 열매", L"Odd Incense", L"Odd Keystone", L"Old Amber", L"Old Charm", L"Old Gateau", L"Old Rod", L"Oran Berry", L"Oval Charm", L"Oval Stone", L"Pair of Tickets", L"Pal Pad", L"Pamtre Berry", L"Paralyze Heal", L"Parcel", L"Park Ball", L"Pass", L"Pass Orb", L"Passho Berry", L"Payapa Berry", L"Pearl", L"Pearl String", L"Pecha Berry", L"Permit", L"시몬 열매", L"Petaya Berry", L"Photo Album", L"Pidgeotite", L"Pikanium Z", L"Pikanium Z", L"Pikashunium Z", L"Pikashunium Z", L"Pinap Berry", L"Pink Apricorn", L"Pink Nectar", L"Pink Scarf", L"쁘사이저 나이트", L"Pixie Plate", L"Plasma Card", L"Plume Fossil", L"Poffin Case", L"Point Card", L"Poison Barb", L"Poison Gem", L"Poison Memory", L"Poisonium Z", L"Poisonium Z", L"Poké Ball", L"Poké Doll", L"Poké Flute", L"Poké Radar", L"Poké Toy", L"Pokéblock Kit", L"Pomeg Berry", L"Potion", L"Power Anklet", L"Power Band", L"Power Belt", L"Power Bracer", L"Power Herb", L"Power Lens", L"Power Plant Pass", L"Power Weight", L"PP Max", L"PP Up", L"Premier Ball", L"Pretty Wing", L"Primarium Z", L"Primarium Z", L"Prism Scale", L"Prison Bottle", L"Prof's Letter", L"Professor's Mask", L"Prop Case", L"Protective Pads", L"Protector", L"Protein", L"Psychic Gem", L"Psychic Memory", L"Psychic Seed", L"Psychium Z", L"Psychium Z", L"Pure Incense", L"Purple Nectar", L"Qualot Berry", L"Quick Ball", L"선제공격손톱", L"Quick Powder", L"Rabuta Berry", L"Rage Candy Bar", L"Rainbow Wing", L"Rare Bone", L"Rare Candy", L"Rawst Berry", L"Razor Claw", L"Razor Fang", L"Razz Berry", L"Reaper Cloth", L"Red Apricorn", L"레드카드", L"Red Chain", L"Red Flute", L"Red Nectar", L"Red Orb", L"Red Scale", L"Red Scarf", L"Red Shard", L"Relic Band", L"Relic Copper", L"Relic Crown", L"Relic Gold", L"Relic Silver", L"Relic Statue", L"Relic Vase", L"Repeat Ball", L"Repel", L"Reply Mail", L"Reset Urge", L"Resist Wing", L"Reveal Glass", L"Revival Herb", L"Revive", L"Rich Mulch", L"Ride Pager", L"Rindo Berry", L"Ring Target", L"Rock Gem", L"Rock Incense", L"Rock Memory", L"Rockium Z", L"Rockium Z", L"울퉁불퉁멧", L"Roller Skates", L"Root Fossil", L"Rose Incense", L"Roseli Berry", L"Rowap Berry", L"RSVP Mail", L"Rule Book", L"S.S. Ticket", L"S.S. Ticket", L"Sablenite", L"Sachet", L"Sacred Ash", L"Safari Ball", L"방진고글", L"Sail Fossil", L"캄라 열매", L"Salamencite", L"Scanner", L"Sceptilite", L"핫삼 나이트", L"Scope Lens", L"Sea Incense", L"Seal Bag", L"Seal Case", L"Secret Key", L"Secret Potion", L"Shalour Sable", L"Sharp Beak", L"Sharpedonite", L"Shed Shell", L"조개껍질방울", L"Shiny Charm", L"Shiny Stone", L"Shoal Salt", L"Shoal Shell", L"Shock Drive", L"Shuca Berry", L"Silk Scarf", L"Silver Powder", L"Silver Wing", L"자몽 열매", L"Skull Fossil", L"Sky Plate", L"Slowbronite", L"Slowpoke Tail", L"Smoke Ball", L"Smooth Rock", L"Snorlium Z", L"Snorlium Z", L"Snowball", L"Soda Pop", L"Soft Sand", L"Soot Sack", L"Soothe Bell", L"Soul Dew", L"Sparkling Stone", L"Spell Tag", L"Spelon Berry", L"Splash Plate", L"Spooky Plate", L"Sport Ball", L"Sprayduck", L"Sprinklotad", L"Squirt Bottle", L"Stable Mulch", L"Star Piece", L"Stardust", L"Starf Berry", L"Steel Gem", L"Steel Memory", L"Steelium Z", L"Steelium Z", L"Steelixite", L"Stick", L"Sticky Barb", L"Stone Plate", L"Storage Key", L"Storage Key", L"Strange Souvenir", L"Suite Key", L"Sun Flute", L"Sun Stone", L"Super Potion", L"Super Repel", L"Super Rod", L"Surprise Mulch", L"Swampertite", L"Sweet Heart", L"Swift Wing", L"Tamato Berry", L"Tanga Berry", L"Tapunium Z", L"Tapunium Z", L"Terrain Extender", L"Thanks Mail", L"Thick Club", L"Thunder Stone", L"Tidal Bell", L"Timer Ball", L"Tiny Mushroom", L"TM01", L"TM02", L"TM03", L"TM04", L"TM05", L"TM06", L"TM07", L"TM08", L"TM09", L"TM10", L"TM100", L"TM11", L"TM12", L"TM13", L"TM14", L"TM15", L"TM16", L"TM17", L"TM18", L"TM19", L"TM20", L"TM21", L"TM22", L"TM23", L"TM24", L"TM25", L"TM26", L"TM27", L"TM28", L"TM29", L"TM30", L"TM31", L"TM32", L"TM33", L"TM34", L"TM35", L"TM36", L"TM37", L"TM38", L"TM39", L"TM40", L"TM41", L"TM42", L"TM43", L"TM44", L"TM45", L"TM46", L"TM47", L"TM48", L"TM49", L"TM50", L"TM51", L"TM52", L"TM53", L"TM54", L"TM55", L"TM56", L"TM57", L"TM58", L"TM59", L"TM60", L"TM61", L"TM62", L"TM63", L"TM64", L"TM65", L"TM66", L"TM67", L"TM68", L"TM69", L"TM70", L"TM71", L"TM72", L"TM73", L"TM74", L"TM75", L"TM76", L"TM77", L"TM78", L"TM79", L"TM80", L"TM81", L"TM82", L"TM83", L"TM84", L"TM85", L"TM86", L"TM87", L"TM88", L"TM89", L"TM90", L"TM91", L"TM92", L"TM93", L"TM94", L"TM95", L"TM96", L"TM97", L"TM98", L"TM99", L"TMV Pass", L"Town Map", L"맹독구슬", L"Toxic Plate", L"Travel Trunk", L"Twisted Spoon", L"마기라스 나이트", L"Ultra Ball", L"Unown Report", L"Up-Grade", L"이상해꽃 나이트", L"Vs. Recorder", L"Vs. Seeker", L"초나 열매", L"Wailmer Pail", L"Water Gem", L"Water Memory", L"Water Stone", L"Waterium Z", L"Waterium Z", L"Watmel Berry", L"Wave Incense", L"약점보험", L"Wepear Berry", L"Whipped Dream", L"White Apricorn", L"White Flute", L"하양허브", L"Wide Lens", L"Wiki Berry", L"Wise Glasses", L"Works Key", L"X Accuracy", L"X Accuracy 2", L"X Accuracy 3", L"X Accuracy 6", L"X Attack", L"X Attack 2", L"X Attack 3", L"X Attack 6", L"X Defense", L"X Defense 2", L"X Defense 3", L"X Defense 6", L"X Sp. Atk", L"X Sp. Atk 2", L"X Sp. Atk 3", L"X Sp. Atk 6", L"X Sp. Def", L"X Sp. Def 2", L"X Sp. Def 3", L"X Sp. Def 6", L"X Speed", L"X Speed 2", L"X Speed 3", L"X Speed 6", L"Xtransceiver", L"Xtransceiver", L"플카 열매", L"Yellow Apricorn", L"Yellow Flute", L"Yellow Nectar", L"Yellow Scarf", L"Yellow Shard", L"Z-Ring", L"Zap Plate", L"Zinc", L"Zoom Lens", L"Zygarde Cube", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", L"???", };
wchar_t *hpList[] = {L"격투", L"비행", L"독", L"땅", L"바위", L"벌레", L"고스트", L"강철", L"불", L"물", L"풀", L"전기", L"에스퍼", L"얼음", L"드래곤", L"악", L" ", L" "};

void GUIElementsInit() {
	unicodeJPN12 = sftd_load_font_file("romfs:/res/UnicodeJPN.otf");
	unicodeKOR12 = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");
	fontBold18 = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");//sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold15 = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");//sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold14 = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");//sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold12 = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");//sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold11 = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");//sftd_load_font_file("romfs:/res/Bold.ttf");
	fontBold9 = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");//sftd_load_font_file("romfs:/res/Bold.ttf");
	fontFixed = sftd_load_font_file("romfs:/res/UnicodeKOR.ttf");//sftd_load_font_file("romfs:/res/VeraMono.ttf");
	warningTop = sfil_load_PNG_file("romfs:/res/Warning Top.png", SF2D_PLACE_RAM);
	warningBottom = sfil_load_PNG_file("romfs:/res/Warning Bottom.png", SF2D_PLACE_RAM);
	freezeMsg("Loading graphics...");
	
	init_font_cache();
	cleanTop = sfil_load_PNG_file("romfs:/res/Clean Top.png", SF2D_PLACE_RAM);
	mainMenuBottom = sfil_load_PNG_file("romfs:/res/Main Menu Bottom.png", SF2D_PLACE_RAM);
}

void GUIElementsSpecify(int game) {
	freezeMsg("Loading graphics...");
	alternativeSpritesSmall = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/alternative_icons_spritesheetv2.png", SF2D_PLACE_RAM);
	spritesSmall = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/pokemon_icons_spritesheetv2.png", SF2D_PLACE_RAM);
	balls = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/balls_spritesheetv2.png", SF2D_PLACE_RAM);
	settings = sfil_load_PNG_file("romfs:/res/Settings.png", SF2D_PLACE_RAM);
	
	if (game < 6) {
		editorBar = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/editor_barv2.png", SF2D_PLACE_RAM);
		typesSheet = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/types_sheetv2.png", SF2D_PLACE_RAM);
		boxView = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/editor_bottomv2.png", SF2D_PLACE_RAM);
		back = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/back_buttonv2.png", SF2D_PLACE_RAM);

		hexIcon = sfil_load_PNG_file("romfs:/res/Hex Button.png", SF2D_PLACE_RAM);
		hexBG = sfil_load_PNG_file("romfs:/res/Hex BG.png", SF2D_PLACE_RAM);
		hexButton = sfil_load_PNG_file("romfs:/res/Selected Byte.png", SF2D_PLACE_RAM);
		blueTextBox = sfil_load_PNG_file("romfs:/res/Blue Textbox.png", SF2D_PLACE_RAM);
		otaButton = sfil_load_PNG_file("romfs:/res/OTA Button.png", SF2D_PLACE_RAM);
		includeInfoButton = sfil_load_PNG_file("romfs:/res/Include Info.png", SF2D_PLACE_RAM);
		generationButton = sfil_load_PNG_file("romfs:/res/Generation Button.png", SF2D_PLACE_RAM);
		generationBG = sfil_load_PNG_file("romfs:/res/Generation BG.png", SF2D_PLACE_RAM);
		hiddenPowerBG = sfil_load_PNG_file("romfs:/res/Hidden Power BG.png", SF2D_PLACE_RAM);
		hiddenPowerButton = sfil_load_PNG_file("romfs:/res/Hidden Power Button.png", SF2D_PLACE_RAM);
		selectBoxButton = sfil_load_PNG_file("romfs:/res/Select Box Button.png", SF2D_PLACE_RAM);
		ballsBG = sfil_load_PNG_file("romfs:/res/BallsBG.png", SF2D_PLACE_RAM);
		ballButton = sfil_load_PNG_file("romfs:/res/Ball Button.png", SF2D_PLACE_RAM);
		male = sfil_load_PNG_file("romfs:/res/Male.png", SF2D_PLACE_RAM);
		female = sfil_load_PNG_file("romfs:/res/Female.png", SF2D_PLACE_RAM);
		naturesButton = sfil_load_PNG_file("romfs:/res/Natures Button.png", SF2D_PLACE_RAM);
		naturestx = sfil_load_PNG_file("romfs:/res/Natures.png", SF2D_PLACE_RAM);
		movesBottom = sfil_load_PNG_file("romfs:/res/Moves Bottom.png", SF2D_PLACE_RAM);
		topMovesBG = sfil_load_PNG_file("romfs:/res/Top Moves.png", SF2D_PLACE_RAM);
		editorStatsBG = sfil_load_PNG_file("romfs:/res/Editor Stats.png", SF2D_PLACE_RAM);
		subArrow = sfil_load_PNG_file("romfs:/res/Sub Arrow.png", SF2D_PLACE_RAM);
		backgroundTop = sfil_load_PNG_file("romfs:/res/Background.png", SF2D_PLACE_RAM);
		miniBox = sfil_load_PNG_file("romfs:/res/Mini Box.png", SF2D_PLACE_RAM);
		minusButton = sfil_load_PNG_file("romfs:/res/Minus Button.png", SF2D_PLACE_RAM);
		plusButton = sfil_load_PNG_file("romfs:/res/Plus Button.png", SF2D_PLACE_RAM);
		transferButton = sfil_load_PNG_file("romfs:/res/Transfer Button.png", SF2D_PLACE_RAM);
		bankTop = sfil_load_PNG_file("romfs:/res/Bank Top.png", SF2D_PLACE_RAM);
		shinyStar = sfil_load_PNG_file("romfs:/res/Shiny.png", SF2D_PLACE_RAM);
		normalBar = sfil_load_PNG_file("romfs:/res/Normal Bar.png", SF2D_PLACE_RAM);
		RButton = sfil_load_PNG_file("romfs:/res/R Button.png", SF2D_PLACE_RAM);
		LButton = sfil_load_PNG_file("romfs:/res/L Button.png", SF2D_PLACE_RAM);
		creditsTop = sfil_load_PNG_file("romfs:/res/Credits Top.png", SF2D_PLACE_RAM);
		pokeball = sfil_load_PNG_file("romfs:/res/Pokeball.png", SF2D_PLACE_RAM);
		menuBar = sfil_load_PNG_file("romfs:/res/Main Menu Dark Bar.png", SF2D_PLACE_RAM);
		menuSelectedBar = sfil_load_PNG_file("romfs:/res/Main Menu Red Bar.png", SF2D_PLACE_RAM);
		darkButton = sfil_load_PNG_file("romfs:/res/Dark Button.png", SF2D_PLACE_RAM);
		eventTop = sfil_load_PNG_file("romfs:/res/Event Top.png", SF2D_PLACE_RAM);
		left = sfil_load_PNG_file("romfs:/res/Left.png", SF2D_PLACE_RAM);
		lightButton = sfil_load_PNG_file("romfs:/res/Light Button.png", SF2D_PLACE_RAM);
		redButton = sfil_load_PNG_file("romfs:/res/Red Button.png", SF2D_PLACE_RAM);
		right = sfil_load_PNG_file("romfs:/res/Right.png", SF2D_PLACE_RAM);
		eventMenuBottomBar = sfil_load_PNG_file("romfs:/res/Event Menu Bottom Bar.png", SF2D_PLACE_RAM);
		eventMenuTopBarSelected = sfil_load_PNG_file("romfs:/res/Event Menu Top Bar Selected.png", SF2D_PLACE_RAM);
		eventMenuTopBar = sfil_load_PNG_file("romfs:/res/Event Menu Top Bar.png", SF2D_PLACE_RAM);
		infoView = sfil_load_PNG_file("romfs:/res/Info View.png", SF2D_PLACE_RAM);
		selector = sfil_load_PNG_file("romfs:/res/Selector.png", SF2D_PLACE_RAM);
		selectorCloning = sfil_load_PNG_file("romfs:/res/Selector (cloning).png", SF2D_PLACE_RAM);
		editorBG = sfil_load_PNG_file("romfs:/res/Editor Bottom BG.png", SF2D_PLACE_RAM);
		plus = sfil_load_PNG_file("romfs:/res/Plus.png", SF2D_PLACE_RAM);
		minus = sfil_load_PNG_file("romfs:/res/Minus.png", SF2D_PLACE_RAM);
		button = sfil_load_PNG_file("romfs:/res/Button.png", SF2D_PLACE_RAM);
		setting = sfil_load_PNG_file("romfs:/res/Setting.png", SF2D_PLACE_RAM);
		bottomPopUp = sfil_load_PNG_file("romfs:/res/Bottom Pop-Up.png", SF2D_PLACE_RAM);
		pokemonBufferBox = sfil_load_PNG_file("romfs:/res/Pokemon Box.png", SF2D_PLACE_RAM);
		topSelectedMove = sfil_load_PNG_file("romfs:/res/Top Selected Move.png", SF2D_PLACE_RAM);
		item = sfil_load_PNG_file("romfs:/res/item.png", SF2D_PLACE_RAM);
	} else {
		DSBottomBG = sfil_load_PNG_file("romfs:/res/Bottom BG.png", SF2D_PLACE_RAM);
		DSTopBG = sfil_load_PNG_file("romfs:/res/Top BG.png", SF2D_PLACE_RAM);
		DSBarSelected = sfil_load_PNG_file("romfs:/res/Bar Selected.png", SF2D_PLACE_RAM);
		DSBar = sfil_load_PNG_file("romfs:/res/Bar.png", SF2D_PLACE_RAM);
		DSEventBottom = sfil_load_PNG_file("romfs:/res/DS Menu Bottom BG.png", SF2D_PLACE_RAM);
		DSLangSelected = sfil_load_PNG_file("romfs:/res/Language Button Selected.png", SF2D_PLACE_RAM);
		DSLang = sfil_load_PNG_file("romfs:/res/Language Button.png", SF2D_PLACE_RAM);
		DSEventTop = sfil_load_PNG_file("romfs:/res/Event Database BG.png", SF2D_PLACE_RAM);
		DSNormalBarL = sfil_load_PNG_file("romfs:/res/Normal L.png", SF2D_PLACE_RAM);
		DSNormalBarR = sfil_load_PNG_file("romfs:/res/Normal R.png", SF2D_PLACE_RAM);
		DSSelectedBarL = sfil_load_PNG_file("romfs:/res/Selected L.png", SF2D_PLACE_RAM);
		DSSelectedBarR = sfil_load_PNG_file("romfs:/res/Selected R.png", SF2D_PLACE_RAM);
	}
}

void GUIGameElementsInit() {
	upperTextGS = sfil_load_PNG_file("romfs:/res/3DS.png", SF2D_PLACE_RAM);
	lowerTextGS = sfil_load_PNG_file("romfs:/res/DS.png", SF2D_PLACE_RAM);
	topBorder = sfil_load_PNG_file("romfs:/res/3dsborder.png", SF2D_PLACE_RAM);
	bottomBorder = sfil_load_PNG_file("romfs:/res/dsborder.png", SF2D_PLACE_RAM);
	gameSelectorBottom = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/dsiconsv2.png", SF2D_PLACE_RAM);
	gameSelectorTop = sfil_load_PNG_file("/3ds/data/PKSM/additionalassets/3dsiconsv2.png", SF2D_PLACE_RAM);
}

void GUIGameElementsExit() {
	sf2d_free_texture(upperTextGS);
	sf2d_free_texture(lowerTextGS);
	sf2d_free_texture(gameSelectorBottom);
	sf2d_free_texture(gameSelectorTop);
	sf2d_free_texture(topBorder);
	sf2d_free_texture(bottomBorder);
}

void GUIElementsExit() {
	sf2d_free_texture(hexIcon);
	sf2d_free_texture(hexBG);
	sf2d_free_texture(hexButton);
	sf2d_free_texture(blueTextBox);
	sf2d_free_texture(otaButton);
	sf2d_free_texture(generationBG);
	sf2d_free_texture(generationButton);
	sf2d_free_texture(includeInfoButton);
	sf2d_free_texture(hiddenPowerBG);
	sf2d_free_texture(hiddenPowerButton);
	sf2d_free_texture(selectBoxButton);
	sf2d_free_texture(ballsBG);
	sf2d_free_texture(ballButton);
	sf2d_free_texture(male);
	sf2d_free_texture(female);
	sf2d_free_texture(naturesButton);
	sf2d_free_texture(naturestx);
	sf2d_free_texture(movesBottom);
	sf2d_free_texture(topMovesBG);
	sf2d_free_texture(editorBar);
	sf2d_free_texture(editorStatsBG);
	sf2d_free_texture(subArrow);
	sf2d_free_texture(backgroundTop);
	sf2d_free_texture(miniBox);
	sf2d_free_texture(minusButton);
	sf2d_free_texture(plusButton);
	sf2d_free_texture(balls);
	sf2d_free_texture(typesSheet);
	sf2d_free_texture(transferButton);
	sf2d_free_texture(bankTop);
	sf2d_free_texture(shinyStar);
	sf2d_free_texture(normalBar);
	sf2d_free_texture(RButton);
	sf2d_free_texture(LButton);
	sf2d_free_texture(creditsTop);
	sf2d_free_texture(pokeball);
	sf2d_free_texture(alternativeSpritesSmall);
	sf2d_free_texture(item);
	sf2d_free_texture(settings);
	sf2d_free_texture(topSelectedMove);
	sf2d_free_texture(DSEventBottom);
	sf2d_free_texture(DSLangSelected);
	sf2d_free_texture(DSLang);
	sf2d_free_texture(DSEventTop);
	sf2d_free_texture(DSNormalBarL);
	sf2d_free_texture(DSNormalBarR);
	sf2d_free_texture(DSSelectedBarL);
	sf2d_free_texture(DSSelectedBarR);
	sf2d_free_texture(DSBottomBG);
	sf2d_free_texture(DSTopBG);
	sf2d_free_texture(DSBarSelected);
	sf2d_free_texture(DSBar);
	sf2d_free_texture(cleanTop);
	sf2d_free_texture(bottomPopUp);
	sf2d_free_texture(pokemonBufferBox);
	sf2d_free_texture(selectorCloning);
	sf2d_free_texture(back);
	sf2d_free_texture(setting);
	sf2d_free_texture(editorBG);
	sf2d_free_texture(plus);
	sf2d_free_texture(minus);
	sf2d_free_texture(button);
	sf2d_free_texture(boxView);
	sf2d_free_texture(infoView);
	sf2d_free_texture(selector);
	sf2d_free_texture(warningTop);
	sf2d_free_texture(warningBottom);
	sf2d_free_texture(eventMenuBottomBar);
	sf2d_free_texture(eventMenuTopBarSelected);
	sf2d_free_texture(eventMenuTopBar);
	sf2d_free_texture(spritesSmall);
	sf2d_free_texture(darkButton);
	sf2d_free_texture(eventTop);
	sf2d_free_texture(left);
	sf2d_free_texture(lightButton);
	sf2d_free_texture(redButton);
	sf2d_free_texture(right);
	sf2d_free_texture(mainMenuBottom);
	sf2d_free_texture(menuBar);
	sf2d_free_texture(menuSelectedBar);
	sftd_free_font(fontBold9);
	sftd_free_font(fontBold11);
	sftd_free_font(fontBold12);
	sftd_free_font(fontBold14);
	sftd_free_font(fontBold15);
	sftd_free_font(fontBold18);
	sftd_free_font(fontFixed);
	sftd_free_font(unicodeJPN12);
	sftd_free_font(unicodeKOR12);
}

void init_font_cache() {
	sftd_draw_text(unicodeJPN12, 0, 0, RGBA8(0, 0, 0, 0), 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	sftd_draw_text(unicodeKOR12, 0, 0, RGBA8(0, 0, 0, 0), 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	sftd_draw_text(fontBold18, 0, 0, RGBA8(0, 0, 0, 0), 18, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	sftd_draw_text(fontBold14, 0, 0, RGBA8(0, 0, 0, 0), 14, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	sftd_draw_text(fontBold15, 0, 0, RGBA8(0, 0, 0, 0), 15, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	sftd_draw_text(fontBold12, 0, 0, RGBA8(0, 0, 0, 0), 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	sftd_draw_text(fontBold11, 0, 0, RGBA8(0, 0, 0, 0), 11, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	sftd_draw_text(fontBold9, 0, 0, RGBA8(0, 0, 0, 0), 9, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
	sftd_draw_text(fontFixed, 0, 0, RGBA8(0, 0, 0, 0), 10, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890:-.'!?()\"end");
}

void drawFPSDebug() {
	sf2d_draw_rectangle(10, 10, 105, 20, RGBA8(0, 0, 0, 200));
	sftd_draw_textf(fontBold12, 12, 13, WHITE, 12, "FPS: %2.6f", sf2d_get_fps());
}

void pksm_end_frame() {
	#if DEBUG
		drawFPSDebug();
	#endif
	sf2d_end_frame();
}

void infoDisp(char* message) {
	while (aptMainLoop()) {
		hidScanInput();

		if (hidKeysDown() & KEY_A) break;
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sf2d_draw_texture(warningTop, 0, 0);
			sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, message)) / 2, 95, RGBA8(255, 255, 255, giveTransparence()), 15, message);
			sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, "Press A to continue.")) / 2, 130, WHITE, 12, "Press A to continue.");
		pksm_end_frame();
		
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_draw_texture(warningBottom, 0, 0);
		pksm_end_frame();
		sf2d_swapbuffers();
	}
}

int confirmDisp(char* message) {
	while (aptMainLoop()) {
		hidScanInput();

		if (hidKeysDown() & KEY_A) return 1;
		if (hidKeysDown() & KEY_B) return 0;
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sf2d_draw_texture(warningTop, 0, 0);
			sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, message)) / 2, 95, RGBA8(255, 255, 255, giveTransparence()), 15, message);
			sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, "Press A to continue, B to cancel.")) / 2, 130, WHITE, 12, "Press A to continue, B to cancel.");
		pksm_end_frame();
		
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_draw_texture(warningBottom, 0, 0);
		pksm_end_frame();
		sf2d_swapbuffers();
	}
	return 0;
}

void freezeMsg(char* message) {
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(warningTop, 0, 0);
		sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, message)) / 2, 95, WHITE, 15, message);
		sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, "Please wait.")) / 2, 130, WHITE, 12, "Please wait.");
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(warningBottom, 0, 0);
	pksm_end_frame();
	sf2d_swapbuffers();
}

void progressBar(char* message, u32 current, u32 sz) {
	char* progress = (char*)malloc(40 * sizeof(char));
	snprintf(progress, 40, "Progress: %lu/%lu bytes", current, sz);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(warningTop, 0, 0);
		sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, message)) / 2, 95, WHITE, 15, message);
		sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, progress)) / 2, 130, WHITE, 12, progress);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(warningBottom, 0, 0);
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(progress);
}

void drawMenuTop(int game) {
	if (game < 6) {
		sf2d_draw_texture(cleanTop, 0, 0);
		printTitle("PKSM");
		sf2d_draw_texture(pokeball, (400 - pokeball->width) / 2 + 5, (240 - pokeball->height) / 2 + 10);
	}
	else 
		sf2d_draw_texture(DSTopBG, 0, 0);
	
	sftd_draw_text(fontBold9, (398 - sftd_get_text_width(fontBold9, 9, VERSION)), 229, LIGHTBLUE, 9, VERSION);
}

void printAnimatedBG(bool isUp) {
	sf2d_draw_texture(backgroundTop, 0, 0);
	animateBG(isUp);
}

void printTitle(const char* title) {
	sftd_draw_text(fontBold14, (400 - sftd_get_text_width(fontBold14, 14, title)) / 2, 4, BLUE, 14, title);
}

void printBottomIndications(const char* message) {
	sftd_draw_text(fontBold9, (320 - sftd_get_text_width(fontBold9, 9, message)) / 2, 225, LIGHTBLUE, 9, message);
}

void gameSelectorMenu(int n) {
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(cleanTop, 0, 0);
		sf2d_draw_texture(upperTextGS, 172, 65);
		
		sftd_draw_text(fontBold9, (400 - sftd_get_text_width(fontBold9, 9, "Cart has priority over digital copy.")) / 2, 6, BLUE, 9, "Cart has priority over digital copy.");
		if (n < 6) 
			sf2d_draw_texture(topBorder, 21 + 60 * n, 112);
		sf2d_draw_texture(gameSelectorTop, 26, 117);
		
		for (int i = 0; i < 6; i++) 
			sftd_draw_wtext(fontBold18, 17 + 60 * i + (68 - sftd_get_wtext_width(fontBold18, 18, (wchar_t*)gamesList[i])) / 2, 170, LIGHTBLUE, 18, (wchar_t*)gamesList[i]);
		
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		sf2d_draw_texture(lowerTextGS, 142, 25);
		switch (n) {
			case GAME_DIAMOND  : { sf2d_draw_texture(bottomBorder, 56, 64); break; }
			case GAME_PEARL    : { sf2d_draw_texture(bottomBorder, 98, 64); break; }
			case GAME_PLATINUM : { sf2d_draw_texture(bottomBorder, 140, 64); break; }
			case GAME_HG       : { sf2d_draw_texture(bottomBorder, 182, 64); break; }
			case GAME_SS	   : { sf2d_draw_texture(bottomBorder, 224, 64); break; }
			case GAME_B1	   : { sf2d_draw_texture(bottomBorder, 77, 123); break; }
			case GAME_W1	   : { sf2d_draw_texture(bottomBorder, 119, 123); break; }
			case GAME_B2	   : { sf2d_draw_texture(bottomBorder, 161, 123); break; }
			case GAME_W2	   : { sf2d_draw_texture(bottomBorder, 203, 123); break; }
		}
		sf2d_draw_texture(gameSelectorBottom, 60, 68);
		
		for (int i = 6; i < 11; i++)
			sftd_draw_wtext(fontBold15, 61 + 42 * (i - 6) + (32 - sftd_get_wtext_width(fontBold15, 15, gamesList[i])) / 2, 104, LIGHTBLUE, 15, gamesList[i]);
		
		for (int i = 11; i < 15; i++)
			sftd_draw_wtext(fontBold15, 82 + 42 * (i - 11) + (32 - sftd_get_wtext_width(fontBold15, 15, gamesList[i])) / 2, 163, LIGHTBLUE, 15, gamesList[i]);

		printBottomIndications("Move your DPAD. Press A to continue, B to exit.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void mainMenu(int currentEntry) {
	wchar_t* menu[3] = {L"EVENTS", L"포켓몬 관리", L"CREDITS"};
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(0);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		sf2d_draw_texture(settings, 292, 194);
		for (int i = 0; i < 3; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(menuSelectedBar, (320 - menuSelectedBar->width) / 2, 60 + i * (menuSelectedBar->height));
			else
				sf2d_draw_texture(menuBar, (320 - menuBar->width) / 2, 60 + i * (menuBar->height));
			sftd_draw_wtext(fontBold18, (320 - sftd_get_wtext_width(fontBold18, 18, menu[i])) / 2 - 4, 53 + (menuBar->height - 18) / 2 + i * (menuBar->height), (i == currentEntry) ? DARKBLUE : YELLOW, 18, menu[i]);
		}
		printBottomIndications("Press START to quit.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void mainMenuDS(int currentEntry) {
	char* menu[2] = {"EVENTS", "OTHER"};
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(7);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(DSBottomBG, 0, 0);
		for (int i = 0; i < 2; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(DSBarSelected, (320 - DSBarSelected->width) / 2, 66 + i * (DSBarSelected->height + 16) + i + 1);
			else
				sf2d_draw_texture(DSBar, (320 - DSBar->width) / 2, 66 + i * (DSBar->height + 16));
			sftd_draw_text(fontBold18, (320 - sftd_get_text_width(fontBold18, 18, menu[i])) / 2, 67 + (DSBar->height - 18) / 2 + i * (DSBar->height + 16), WHITE, 18, menu[i]);
		}
		printBottomIndications("Press START to quit.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void menu4(int currentEntry, wchar_t* menu[], int n) {
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(0);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		
		for (int i = 0; i < n; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(menuSelectedBar, (320 - menuSelectedBar->width) / 2, 40 + i * (menuSelectedBar->height));
			else
				sf2d_draw_texture(menuBar, (320 - menuBar->width) / 2, 40 + i * (menuBar->height));
			sftd_draw_wtext(fontBold18, (320 - sftd_get_wtext_width(fontBold18, 18, menu[i])) / 2 - 4, 44 + i * (menuBar->height), (i == currentEntry) ? DARKBLUE : YELLOW, 18, menu[i]);
		}
		printBottomIndications("Press A to select an option.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void menu3(int currentEntry, char* menu[], int n) {
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(0);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		
		for (int i = 0; i < 3; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(menuSelectedBar, (320 - menuSelectedBar->width) / 2, 60 + i * (menuSelectedBar->height));
			else
				sf2d_draw_texture(menuBar, (320 - menuBar->width) / 2, 60 + i * (menuBar->height));
			sftd_draw_text(fontBold18, (320 - sftd_get_text_width(fontBold18, 18, menu[i])) / 2 - 4, 53 + (menuBar->height - 18) / 2 + i * (menuBar->height), (i == currentEntry) ? DARKBLUE : YELLOW, 18, menu[i]);
		}
		printBottomIndications("Press A to select an option.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printCredits() {
	u8* buf = (u8*)malloc(1500 * sizeof(u8));
	loadFile(buf, "romfs:/res/credits.txt");
	
	while (aptMainLoop() && !(hidKeysUp() & KEY_B)) {
		hidScanInput();
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			sf2d_draw_texture(cleanTop, 0, 0);
			printTitle("Credits");
			sf2d_draw_texture(creditsTop, 0, 45);
			sftd_draw_text(fontBold15, 18, 77, LIGHTBLUE, 15,  "Bernardo Giordano");
			sftd_draw_text(fontBold15, 64, 174, LIGHTBLUE, 15,  "dsoldier for the complete GUI design");
		pksm_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			sf2d_draw_texture(mainMenuBottom, 0, 0);
			sftd_draw_text(fontBold9, 20, 30, LIGHTBLUE, 9, (char*)buf);
			printBottomIndications("Press B to return.");
		pksm_end_frame();
		sf2d_swapbuffers();
	}
	
	free(buf);
}

void printDatabase6(char *database[], int currentEntry, int page, int spriteArray[]) {
	int pk, y = 41;
	char *pages = (char*)malloc(10 * sizeof(char));
	snprintf(pages, 10, "%d/%d", page + 1, 205);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(cleanTop, 0, 0);
		printTitle("Event Database");
		
		for (int i = 0; i < 5; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 18, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 18, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 20, y - ((i == currentEntry) ? movementOffsetSlow(2) : 0), 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 54, y + 14, (i == currentEntry) ? HIGHBLUE : YELLOW, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 54, y + 3, (i == currentEntry) ? HIGHBLUE : YELLOW, 9, 148, database[page * 10 + i]);
			
			y += 37;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 200, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 200, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 202, y - ((i == currentEntry) ? movementOffsetSlow(2) : 0), 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 235, y + 14, (i == currentEntry) ? HIGHBLUE : YELLOW, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 235, y + 3, (i == currentEntry) ? HIGHBLUE : YELLOW, 9, 148, database[page * 10 + i]);
			
			y += 37;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		sf2d_draw_texture(eventMenuBottomBar, (320 - eventMenuBottomBar->width) / 2, 45);
		sf2d_draw_texture(LButton, 83, 52);
		sf2d_draw_texture(RButton, 221, 52);
		sftd_draw_text(fontBold12, (320 - sftd_get_text_width(fontBold12, 12, pages)) / 2, 52, WHITE, 12, pages);
		printBottomIndications("Press A to continue, B to return.");
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(pages);
}

void printDatabase5(char *database[], int currentEntry, int page, int spriteArray[], bool isSelected, int langSelected, bool langVett[]) {
	int pk, y = 41;
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(DSEventTop, 0, 0);
		
		for (int i = 0; i < 5; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(DSSelectedBarL, 18, y);
			else
				sf2d_draw_texture(DSNormalBarL, 18, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 22, y + 2, 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 55, y + 14, WHITE, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 55, y + 3, WHITE, 9, 148, database[page * 10 + i]);
			
			y += DSSelectedBarL->height;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(DSSelectedBarR, 200, y);
			else
				sf2d_draw_texture(DSNormalBarR, 200, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 204, y + 2, 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 235, y + 14, WHITE, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 235, y + 3, WHITE, 9, 148, database[page * 10 + i]);
			
			y += DSSelectedBarR->height;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(DSEventBottom, 0, 0);
		sftd_draw_text(fontBold9, (320 - sftd_get_text_width(fontBold9, 9, isSelected ? "Press START to inject, B to return" : "Press A to select, B to return.")) / 2, 222, RGBA8(255, 255, 255, 130), 9, isSelected ? "Press START to inject, B to return" : "Press A to select, B to return.");
		
		if (isSelected) {
			char *languages[7] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR"};
			char *path = (char*)malloc(40 * sizeof(char));
			u8* buf = (u8*)malloc(1500 * sizeof(u8));
			memset(buf, 0, 1500);
			snprintf(path, 40, "romfs:/database/gen5/%d.txt", page * 10 + currentEntry);
			loadFile(buf, path);
			
			sftd_draw_text(fontFixed, 8, 2,  WHITE, 10, (char*)buf);
			
			for (int t = 0; t < 7; t++) {
				int x = 0, y = 178;
				switch (t) {
					case 0 : { x = 25; break; }
					case 1 : { x = 63; break; }
					case 2 : { x = 101; break; }
					case 3 : { x = 139; break; }
					case 4 : { x = 177; break; }
					case 5 : { x = 215; break; }
					case 6 : { x = 253; break; }
				}
				
				sf2d_draw_texture(DSLang, x, y);
				if (langVett[t]) {
					if (t == langSelected) sf2d_draw_texture(DSLangSelected, x, y);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[t])) / 2, y + 4, WHITE, 12, languages[t]);
				}
				else {
					if (t == langSelected) sf2d_draw_texture(DSLangSelected, x, y);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[t])) / 2, y + 4, RGBA8(255, 255, 255, 100), 12, languages[t]);
				}
			}

			free(path);
			free(buf);
		}
	
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printDatabase4(char *database[], int currentEntry, int page, int spriteArray[], bool isSelected, int langSelected, bool langVett[]) {
	int pk, y = 41;
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(DSEventTop, 0, 0);
		
		for (int i = 0; i < 5; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(DSSelectedBarL, 18, y);
			else
				sf2d_draw_texture(DSNormalBarL, 18, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 22, y + 2, 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 55, y + 14, WHITE, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 55, y + 3, WHITE, 9, 148, database[page * 10 + i]);
			
			y += DSSelectedBarL->height;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			pk = spriteArray[page * 10 + i];
			if (i == currentEntry)
				sf2d_draw_texture(DSSelectedBarR, 200, y);
			else
				sf2d_draw_texture(DSNormalBarR, 200, y);
			
			if (pk != -1)
				sf2d_draw_texture_part(spritesSmall, 204, y + 2, 40 * (pk % 25) + 4, 30 * (pk / 25), 34, 30);
			if (sftd_get_text_width(fontBold9, 9, database[page * 10 + i]) <= 148)
				sftd_draw_text(fontBold9, 235, y + 14, WHITE, 9, database[page * 10 + i]);
			else
				sftd_draw_text_wrap(fontBold9, 235, y + 3, WHITE, 9, 148, database[page * 10 + i]);
			
			y += DSSelectedBarR->height;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(DSEventBottom, 0, 0);
		sftd_draw_text(fontBold9, (320 - sftd_get_text_width(fontBold9, 9, isSelected ? "Press START to inject, B to return" : "Press A to select, B to return.")) / 2, 222, RGBA8(255, 255, 255, 130), 9, isSelected ? "Press START to inject, B to return" : "Press A to select, B to return.");
		
		if (isSelected) {
			char *languages[7] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR"};
			char *path = (char*)malloc(40 * sizeof(char));
			u8* buf = (u8*)malloc(1500 * sizeof(u8));
			memset(buf, 0, 1500);
			snprintf(path, 40, "romfs:/database/gen4/%d.txt", page * 10 + currentEntry);
			loadFile(buf, path);
			
			sftd_draw_text(fontFixed, 8, 2,  WHITE, 10, (char*)buf);
			
			for (int t = 0; t < 7; t++) {
				int x = 0, y = 178;
				switch (t) {
					case 0 : { x = 25; break; }
					case 1 : { x = 63; break; }
					case 2 : { x = 101; break; }
					case 3 : { x = 139; break; }
					case 4 : { x = 177; break; }
					case 5 : { x = 215; break; }
					case 6 : { x = 253; break; }
				}
				
				sf2d_draw_texture(DSLang, x, y);
				if (langVett[t]) {
					if (t == langSelected) sf2d_draw_texture(DSLangSelected, x, y);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[t])) / 2, y + 4, WHITE, 12, languages[t]);
				}
				else {
					if (t == langSelected) sf2d_draw_texture(DSLangSelected, x, y);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[t])) / 2, y + 4, RGBA8(255, 255, 255, 100), 12, languages[t]);
				}
			}

			free(path);
			free(buf);
		}
	
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printDB7(int sprite, int i, bool langVett[], bool adapt, bool overwrite, int langSelected, int nInjected) {
	char *languages[] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR", "CHS", "CHT"};
	char *path = (char*)malloc(30 * sizeof(char));
	char *cont = (char*)malloc(3 * sizeof(char));
	u8* buf = (u8*)malloc(1500 * sizeof(u8));
	memset(buf, 0, 1499);
	snprintf(path, 30, "romfs:/database/%d.txt", i);
	snprintf(cont, 3, "%d", nInjected + 1);
	loadFile(buf, path);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		printAnimatedBG(true);
		sf2d_draw_texture(eventTop, 0, 24);
		if (sprite != -1)
			sf2d_draw_texture_part_scale(spritesSmall, 282, 46 - movementOffsetLong(6), 40 * (sprite % 25) + 4, 30 * (sprite / 25), 34, 30, 2, 2);
		sftd_draw_text(fontFixed, 5, 28,  WHITE, 10, (char*)buf);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		sftd_draw_text(fontBold14, 16, 50, LIGHTBLUE, 14, "Languages:");
		sftd_draw_text(fontBold14, 33, 112, LIGHTBLUE, 14, "Overwrite Wondercard:");
		sftd_draw_text(fontBold14, 33, 140, LIGHTBLUE, 14, "Adapt language to WC:");
		sftd_draw_text(fontBold14, 33, 170, LIGHTBLUE, 14, "Inject WC in slot:");
		
		for (int t = 0; t < 9; t++) {
			int x = 0, y = 0;
			switch (t) {
				case 0 : { x = 114; y = 50; break; }
				case 1 : { x = 153; y = 50; break; }
				case 2 : { x = 192; y = 50; break; }
				case 3 : { x = 231; y = 50; break; }
				case 4 : { x = 270; y = 50; break; }
				case 5 : { x = 133; y = 74; break; }
				case 6 : { x = 172; y = 74; break; }
				case 7 : { x = 211; y = 74; break; }
				case 8 : { x = 250; y = 74; break; }
			}
			if (langVett[t]) {
				if (t == langSelected) {
					sf2d_draw_texture(redButton, x, y);
					sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, DARKBLUE, 14, languages[t]);
				} else {
					sf2d_draw_texture(darkButton, x, y);
					sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, YELLOW, 14, languages[t]);
				}
			}
			else {
				sf2d_draw_texture(lightButton, x, y);
				sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, DARKBLUE, 14, languages[t]);
			}
		}
			
		if (overwrite) {
			sf2d_draw_texture(redButton, 210, 110);
			sf2d_draw_texture(darkButton, 249, 110);
		}
		else {
			sf2d_draw_texture(darkButton, 210, 110);
			sf2d_draw_texture(redButton, 249, 110);			
		}
		
		if (adapt) {
			sf2d_draw_texture(redButton, 210, 138);
			sf2d_draw_texture(darkButton, 249, 138);
		}
		else {
			sf2d_draw_texture(darkButton, 210, 138);
			sf2d_draw_texture(redButton, 249, 138);
		}
		
		sf2d_draw_texture(darkButton, 229, 168);	
		
		sftd_draw_text(fontBold14, 210 + (36 - sftd_get_text_width(fontBold14, 14, "Yes")) / 2, 112, (overwrite) ? DARKBLUE : YELLOW, 14, "Yes");
		sftd_draw_text(fontBold14, 249 + (36 - sftd_get_text_width(fontBold14, 14, "No")) / 2, 112, (!overwrite) ? DARKBLUE : YELLOW, 14, "No");
		sftd_draw_text(fontBold14, 210 + (36 - sftd_get_text_width(fontBold14, 14, "Yes")) / 2, 140, (adapt) ? DARKBLUE : YELLOW, 14, "Yes");
		sftd_draw_text(fontBold14, 249 + (36 - sftd_get_text_width(fontBold14, 14, "No")) / 2, 140, (!adapt) ? DARKBLUE : YELLOW, 14, "No");
		sftd_draw_text(fontBold14, 229 + (36 - sftd_get_text_width(fontBold14, 14, cont)) / 2, 170, YELLOW, 14, cont);
		
		printBottomIndications("Press START to inject, B to return.");
		pksm_end_frame();
	sf2d_swapbuffers();
	
	free(buf);
	free(cont);
	free(path);
}

void printDB6(int sprite, int i, bool langVett[], bool adapt, bool overwrite, int langSelected, int nInjected) {
	char *languages[7] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR"};
	char *path = (char*)malloc(30 * sizeof(char));
	char *cont = (char*)malloc(3 * sizeof(char));
	u8* buf = (u8*)malloc(1500 * sizeof(u8));
	memset(buf, 0, 1499);
	snprintf(path, 30, "romfs:/database/%d.txt", i);
	snprintf(cont, 3, "%d", nInjected + 1);
	loadFile(buf, path);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		printAnimatedBG(true);
		sf2d_draw_texture(eventTop, 0, 24);
		if (sprite != -1)
			sf2d_draw_texture_part_scale(spritesSmall, 282, 46 - movementOffsetLong(6), 40 * (sprite % 25) + 4, 30 * (sprite / 25), 34, 30, 2, 2);
		sftd_draw_text(fontFixed, 5, 28,  WHITE, 10, (char*)buf);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		sftd_draw_text(fontBold14, 33, 50, LIGHTBLUE, 14, "Languages:");
		sftd_draw_text(fontBold14, 33, 112, LIGHTBLUE, 14, "Overwrite Wondercard:");
		sftd_draw_text(fontBold14, 33, 140, LIGHTBLUE, 14, "Adapt language to WC:");
		sftd_draw_text(fontBold14, 33, 170, LIGHTBLUE, 14, "Inject WC in slot:");
		
		for (int t = 0; t < 7; t++) {
			int x = 0, y = 0;
			switch (t) {
				case 0 : { x = 132; y = 50; break; }
				case 1 : { x = 171; y = 50; break; }
				case 2 : { x = 210; y = 50; break; }
				case 3 : { x = 249; y = 50; break; }
				case 4 : { x = 151; y = 74; break; }
				case 5 : { x = 190; y = 74; break; }
				case 6 : { x = 229; y = 74; break; }
			}
			if (langVett[t]) {
				if (t == langSelected) {
					sf2d_draw_texture(redButton, x, y);
					sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, DARKBLUE, 14, languages[t]);
				} else {
					sf2d_draw_texture(darkButton, x, y);
					sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, YELLOW, 14, languages[t]);
				}
			}
			else {
				sf2d_draw_texture(lightButton, x, y);
				sftd_draw_text(fontBold14, x + (36 - sftd_get_text_width(fontBold14, 14, languages[t])) / 2, y + 2, DARKBLUE, 14, languages[t]);
			}
		}
			
		if (overwrite) {
			sf2d_draw_texture(redButton, 210, 110);
			sf2d_draw_texture(darkButton, 249, 110);
		}
		else {
			sf2d_draw_texture(darkButton, 210, 110);
			sf2d_draw_texture(redButton, 249, 110);			
		}
		
		if (adapt) {
			sf2d_draw_texture(redButton, 210, 138);
			sf2d_draw_texture(darkButton, 249, 138);
		}
		else {
			sf2d_draw_texture(darkButton, 210, 138);
			sf2d_draw_texture(redButton, 249, 138);
		}
		
		sf2d_draw_texture(darkButton, 229, 168);	
		
		sftd_draw_text(fontBold14, 210 + (36 - sftd_get_text_width(fontBold14, 14, "Yes")) / 2, 112, (overwrite) ? DARKBLUE : YELLOW, 14, "Yes");
		sftd_draw_text(fontBold14, 249 + (36 - sftd_get_text_width(fontBold14, 14, "No")) / 2, 112, (!overwrite) ? DARKBLUE : YELLOW, 14, "No");
		sftd_draw_text(fontBold14, 210 + (36 - sftd_get_text_width(fontBold14, 14, "Yes")) / 2, 140, (adapt) ? DARKBLUE : YELLOW, 14, "Yes");
		sftd_draw_text(fontBold14, 249 + (36 - sftd_get_text_width(fontBold14, 14, "No")) / 2, 140, (!adapt) ? DARKBLUE : YELLOW, 14, "No");
		sftd_draw_text(fontBold14, 229 + (36 - sftd_get_text_width(fontBold14, 14, cont)) / 2, 170, YELLOW, 14, cont);
		
		printBottomIndications("Press START to inject, B to return.");
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(buf);
	free(cont);
	free(path);
}

void printEditor(u8* mainbuf, int game, int currentEntry, int langCont) {
	int y = 41;
	char *languages[] = {"JPN", "ENG", "FRE", "ITA", "GER", "SPA", "KOR", "CHS", "CHT"};

	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(cleanTop, 0, 0);
		printTitle("Save File Editor");
		
		for (int i = 0; i < 5; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 18, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 18, y);
			
			switch (i) {
				case 0 : {
					int x = 140;
					sftd_draw_text(fontBold12, 40, y + 10, BLACK, 12, "Language:");
					sf2d_draw_texture(miniBox, x, y + 7);
					sftd_draw_text(fontBold12, x + (36 - sftd_get_text_width(fontBold12, 12, languages[langCont])) / 2, y + 10, YELLOW, 12, languages[langCont]);
					break;
				}
			}
			y += 37;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 200, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 200, y);
			
			switch (i) {
				case 5 : { sftd_draw_text(fontBold12, 200 + (182 - sftd_get_text_width(fontBold12, 12, "Clear Mystery Gift box")) / 2, y + 10, (i == currentEntry) ? DARKBLUE : YELLOW, 12, "Clear Mystery Gift box"); break; }
			}
			y += 37;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		printBottomIndications("Press START to edit, A to toggle, B to exit.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

u16 getAlternativeSprite(u8* pkmn, int game) {
	u8 form = getForm(pkmn);
	if (form) {
		FormData *forms = getLegalFormData(getPokedexNumber(pkmn), game);
		int spritenum = forms->spriteNum;
		if (spritenum > 0 && form >= forms->min && form <= forms->max)
			spritenum += form - (forms->min > 0 ? forms->min : 1);
		free(forms);
		return spritenum;
	}
	return 0;
}

void printElementFX(u16 n, int x, int y) {
	sf2d_draw_texture_part(spritesSmall, x, y, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30);
}

void printElement(u8* mainbuf, u8* pkmn, int game, u16 n, int x, int y) {
	if (!areMarksZero(pkmn, game) && (game == GAME_SUN || game == GAME_MOON))
		printElementBlend(pkmn, game, n, x, y, SHINYRED);
	else {
		u16 t = getAlternativeSprite(pkmn, game);
		if (t) {
			t -= 1;
			sf2d_draw_texture_part(alternativeSpritesSmall, x, y, 40 * (t % 6) + 4, 30 * (t / 6), 34, 30); 
		} else {
			if (getPokedexNumber(pkmn) < 0 || getPokedexNumber(pkmn) > 821)
				sf2d_draw_texture_part(spritesSmall, x, y, 0, 0, 34, 30);
			else
				sf2d_draw_texture_part(spritesSmall, x, y, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30);
		}
		if (isEgg(pkmn))
			sf2d_draw_texture_part(spritesSmall, x + 6, y + 6, 40 * (EGGSPRITEPOS % 25) + 4, 30 * (EGGSPRITEPOS / 25), 34, 30);
		if (getItem(pkmn))
			sf2d_draw_texture(item, x + 3, y + 21);
	}
}
void printElementBlend(u8* pkmn, int game, u16 n, int x, int y, u32 color) {
	u16 t = getAlternativeSprite(pkmn, game);
	if (t) {
		t -= 1;
		sf2d_draw_texture_part_blend(alternativeSpritesSmall, x, y, 40 * (t % 6) + 4, 30 * (t / 6), 34, 30, color); 
	} else {
		if (getPokedexNumber(pkmn) < 0 || getPokedexNumber(pkmn) > 821)
			sf2d_draw_texture_part_blend(spritesSmall, x, y, 0, 0, 34, 30, color);
		else
			sf2d_draw_texture_part_blend(spritesSmall, x, y, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30, color);
	}
	if (isEgg(pkmn))
		sf2d_draw_texture_part_blend(spritesSmall, x + 6, y + 6, 40 * (EGGSPRITEPOS % 25) + 4, 30 * (EGGSPRITEPOS / 25), 34, 30, color);
	if (getItem(pkmn))
		sf2d_draw_texture_blend(item, x + 3, y + 21, color);
}

void infoViewer(u8* pkmn, int game) {
	bool isKor = (pkmn[0xE3] == 0x08) ? true : false;
	int y_desc = 29;
	wchar_t* entries[] = {L"닉네임:", L"부모:", L"포케러스", L"성격", L"특성:", L"지닌물건:", L"ESV / TSV:", L"TID / SID:", L"HT/OT Fr.ship:", L"숨겨진힘:"};
	wchar_t* values[] = {L"체", L"공", L"방", L"특공", L"특방", L"스피드"};
	
	printAnimatedBG(true);
	sf2d_draw_texture(infoView, 0, 2);

	sf2d_draw_texture(normalBar, 252, 155);
	sf2d_draw_texture(normalBar, 252, 176);
	sf2d_draw_texture(normalBar, 252, 197);
	sf2d_draw_texture(normalBar, 252, 218);
	
	sftd_draw_wtext(fontBold12, 251, 138, WHITE, 12, L"기술");
	for (int i = 0; i < 10; i++) {
		sftd_draw_wtext(fontBold12, 2, y_desc, BLUE, 12, entries[i]);
		y_desc += 20;
		if (i == 2) y_desc += 5;
		if (i == 5) y_desc += 6;
	}
	
	y_desc = 8;
	for (int i = 0; i < 6; i++) {
		sftd_draw_wtext(fontBold12, 225, y_desc, LIGHTBLUE, 12, values[i]);
		y_desc += 20;
	}
	
	if (getPokedexNumber(pkmn) > 0 && getPokedexNumber(pkmn) < 822) {
		sf2d_draw_texture_part(balls, -2, -5, 32 * (getBall(pkmn) % 8), 32 * (getBall(pkmn) / 8), 32, 32);
		sftd_draw_wtext(fontBold12, 30, 6, WHITE, 12, personal.species[getPokedexNumber(pkmn)]);
		
		if (getGender(pkmn) == 0)
			sf2d_draw_texture(male, 146, 7);
		else if (getGender(pkmn) == 1)
			sf2d_draw_texture(female, 148, 7);
		
		char* level = (char*)malloc(8 * sizeof(char));
		snprintf(level, 8, "Lv.%u", getLevel(pkmn));
		sftd_draw_text(fontBold12, 160, 6, WHITE, 12, level);
		free(level);
		
		u32 nick[NICKNAMELENGTH*2];
		memset(nick, 0, NICKNAMELENGTH*2);
		getNickname(pkmn, nick);
		sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 215 - (sftd_get_wtext_width((isKor) ? unicodeKOR12 : unicodeJPN12, 12, (wchar_t*)nick)), 29, WHITE, 12, (wchar_t*)nick);
		
		u32 ot_name[NICKNAMELENGTH*2];
		memset(ot_name, 0, NICKNAMELENGTH*2);
		getOT(pkmn, ot_name);
		sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 215 - (sftd_get_wtext_width((isKor) ? unicodeKOR12 : unicodeJPN12, 12, (wchar_t*)ot_name)), 49, WHITE, 12, (wchar_t*)ot_name);
		
		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, (isInfected(pkmn) ? L"O" : L"X")), 69, WHITE, 12, isInfected(pkmn) ? L"O" : L"X");
		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, natures[getNature(pkmn)]), 94, WHITE, 12, natures[getNature(pkmn)]);
		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, abilities[getAbility(pkmn)]), 114, WHITE, 12, abilities[getAbility(pkmn)]);
		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, items[getItem(pkmn)]), 134, WHITE, 12, items[getItem(pkmn)]);
		
		if (isShiny(pkmn))
			sf2d_draw_texture(shinyStar, 205, 9);
		
		char* friendship = (char*)malloc(11 * sizeof(char));
		snprintf(friendship, 11, "%u / %u", getFriendship(pkmn), getOTFriendship(pkmn));
		sftd_draw_text(fontBold12, 215 - sftd_get_text_width(fontBold12, 12, friendship), 200, WHITE, 12, friendship);
		free(friendship);
		
		char* otid = (char*)malloc(18 * sizeof(char));
		snprintf(otid, 18, "%u / %u", getPSV(pkmn), getTSV(pkmn));
		sftd_draw_text(fontBold12, 215 - sftd_get_text_width(fontBold12, 12, otid), 160, WHITE, 12, otid);
		snprintf(otid, 18, "%u / %u", getOTID(pkmn), getSOTID(pkmn));
		sftd_draw_text(fontBold12, 215 - sftd_get_text_width(fontBold12, 12, otid), 180, WHITE, 12, otid);
		free(otid);

		sftd_draw_wtext(fontBold12, 215 - sftd_get_wtext_width(fontBold12, 12, hpList[getHPType(pkmn)]), 220, WHITE, 12, hpList[getHPType(pkmn)]);
		
		int max = sftd_get_text_width(fontBold12, 12, "252");		
		int y_moves = 159;
		for (int i = 0; i < 4; i++) {
			if (getMove(pkmn, i))
				sftd_draw_wtext(fontBold12, 396 - sftd_get_wtext_width(fontBold12, 12, moves[getMove(pkmn, i)]), y_moves, WHITE, 12, moves[getMove(pkmn, i)]);
			y_moves += 21;
		}
		
		char* tmp = (char*)malloc(4);
		for (int i = 0; i < 6; i++) {
			snprintf(tmp, 4, "%d", getIV(pkmn, lookup[i]));
			sftd_draw_text(fontBold12, 289 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 8 + i * 20, WHITE, 12, tmp);
			snprintf(tmp, 4, "%d", getEV(pkmn, lookup[i]));
			sftd_draw_text(fontBold12, 328 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 8 + i * 20, WHITE, 12, tmp);
			snprintf(tmp, 4, "%d", getStat(pkmn, lookup[i]));
			sftd_draw_text(fontBold12, 369 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 8 + i * 20, WHITE, 12, tmp);
		}
		free(tmp);
	}
}

void printPKViewer(u8* mainbuf, u8* tmp, bool isTeam, int game, int currentEntry, int menuEntry, int box, int mode, int additional1, int additional2) {
	wchar_t* menuEntries[] = {L"수정", L"복사", L"놓아주기", L"바꾸기", L"종료"};
	int x;
	char* page = (char*)malloc(7 * sizeof(char));
	snprintf(page, 7, "Box %d", box + 1);
	
	u8* pkmn = (u8*)malloc(PKMNLENGTH * sizeof(u8));
	getPkmn(mainbuf, (isTeam) ? 33 : box, currentEntry, pkmn, game);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		if (mode == ED_GENERATE) {
			char* temp = (char*)malloc(4);
			sf2d_draw_texture(generationBG, 0, 0);
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 8; j++) {
					if (additional1 == i * 8 + j)
						sf2d_draw_texture(generationButton, j * 49 + j, + i * 47 + i);
					sf2d_draw_texture_part(spritesSmall, 7 + 49 * j + j, 2 + 47 * i + i, 40 * ((40 * additional2 + i * 8 + j + 1) % 25) + 4, 30 * ((40 * additional2 + i * 8 + j + 1) / 25), 34, 30);
					snprintf(temp, 4, "%d", 40 * additional2 + i * 8 + j + 1);
					sftd_draw_text(fontBold9, 49 * j + (49 - sftd_get_text_width(fontBold9, 9, temp)) / 2 + j, 34 + i * 47 + i, WHITE, 9, temp);
				}
			}
			free(temp);
		} else
			infoViewer(pkmn, game);
		
		if (mode == ED_OTA) {
			sf2d_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, 220));
			sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, "Launch the client on your PC...")) / 2, 95, RGBA8(255, 255, 255, giveTransparence()), 15, "Launch the client on your PC...");
			sftd_draw_text(fontBold12, (400 - sftd_get_text_width(fontBold12, 12, "Press B to exit.")) / 2, 130, WHITE, 12, "Press B to exit.");
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(boxView, 0, 0);
		sf2d_draw_texture(editorBar, 0, 210);
		sftd_draw_text(fontBold12, 12 + (178 - sftd_get_text_width(fontBold12, 12, page)) / 2, 19, WHITE, 12, page);
		sf2d_draw_texture(left, 7, 17);
		sf2d_draw_texture(right, 185, 17);
		sf2d_draw_texture(otaButton, 244, 210);
		sf2d_draw_texture(back, 280, 210);
		sftd_draw_text(fontBold12, 247, 7, WHITE, 12, "Team");
		
		int y = 45;
		int pointer[2] = {0, 0};
		for (int i = 0; i < 5; i++) {
			x = 4;
			for (int j = 0; j < 6; j++) {
				getPkmn(mainbuf, box, i * 6 + j, pkmn, game);
				u16 n = getPokedexNumber(pkmn);
				if (n > 0 && n < 822)
					printElement(mainbuf, pkmn, game, n, x, y);

				if ((currentEntry == (i * 6 + j)) && !isTeam) {
					pointer[0] = x + 18;
					pointer[1] = y - 8;
				}
				x += 34;
			}
			y += 30;
		}
		
		for (int i = 0; i < 3; i++) {
			sf2d_draw_texture(pokemonBufferBox, 214, 40 + i * 45);
			sf2d_draw_texture(pokemonBufferBox, 266, 60 + i * 45);
		}
		
		y = 45;
		for (int i = 0; i < 3; i++) {
			x = 222;
			for (int j = 0; j < 2; j++) {
				getPkmn(mainbuf, 33, i * 2 + j, pkmn, game);
				u16 n = getPokedexNumber(pkmn);
				if (n)
					printElement(mainbuf, pkmn, game, n, x, (j == 1) ? y + 20 : y);

				if ((currentEntry == (i * 2 + j)) && isTeam) {
					pointer[0] = x + 18;
					pointer[1] = y - 8 + ((j == 1) ? 20 : 0);
				}
				x += 52;
			}
			y += 45;
		}
		if (mode != ED_CLONE)
			sf2d_draw_texture(selector, pointer[0], pointer[1] - ((mode == ED_STANDARD) ? movementOffsetSlow(3) : 0));
		else {
			sf2d_draw_texture(selectorCloning, pointer[0], pointer[1] - movementOffsetSlow(3));			
			sf2d_draw_texture(bottomPopUp, 1, 214);
			sftd_draw_text(fontBold11, 8, 220, WHITE, 11, "Press A to clone in selected slot, B to cancel");
		}
		
		if (mode == ED_MENU) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sf2d_draw_texture(bottomPopUp, 1, 214);
			sf2d_draw_texture(includeInfoButton, 242, 5);
			sftd_draw_wtextf(fontBold11, 8, 220, WHITE, 11, L"%ls 선택됨", personal.species[getPokedexNumber(tmp)]);
			for (int i = 0; i < 5; i++) {
				sf2d_draw_texture(button, 208, 42 + i * 27);
				if (isTeam && (i == 0 || i == 2 || i == 3))
					sf2d_draw_texture_blend(button, 208, 42 + i * 27, RGBA8(0, 0, 0, 100));
				sftd_draw_wtext(fontBold12, 208 + (109 - sftd_get_wtext_width(fontBold12, 12, menuEntries[i])) / 2, 49 + i * 27, BLACK, 12, menuEntries[i]);
				if (i == menuEntry)
					sf2d_draw_texture(subArrow, 203 - movementOffsetSlow(3), 46 + i * 27);
			}
		} else if (mode == ED_GENERATE) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Generate with A in the top screen.")) / 2, 105, WHITE, 14, "Generate with A in the top screen.");
		} else if (mode == ED_OTA) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			drawIP(fontBold9);
		} else if (mode != ED_CLONE) {
			if (mode == ED_STANDARD)
				sftd_draw_textf(fontBold9, 16, 220, WHITE, 9, "TID: %u / SID: %u / TSV: %u", getSaveTID(mainbuf, game), getSaveSID(mainbuf, game), getSaveTSV(mainbuf, game));	
			else
				sftd_draw_textf(fontBold9, 16, 220, WHITE, 9, "Seed: %lX %lX %lX %lX", getSaveSeed(mainbuf, game, 3), getSaveSeed(mainbuf, game, 2), getSaveSeed(mainbuf, game, 1), getSaveSeed(mainbuf, game, 0));	
		}
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(pkmn);
	free(page);
}

void printPKEditor(u8* pkmn, int game, int additional1, int additional2, int additional3, int mode, char* descriptions[]) {
	int max = sftd_get_text_width(fontBold12, 12, "252");
	wchar_t* entries[] = {L"레벨:", L"성격:", L"특성:", L"아이템:", L"황금:", L"포케러스:", L"부모:", L"별명:", L"친분:"};
	wchar_t* options[] = {L"스탯", L"기술", L"저장"};
	
	wchar_t* values[6] = {L"체:", L"공:", L"방:", L"특공:", L"특방:", L"스피드:"};
	u16 n = getPokedexNumber(pkmn);
	bool isKor = (pkmn[0xE3] == 0x08) ? true : false;
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	if (mode == ED_BASE || mode == ED_STATS) {
		sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, "Edit your Pokemon in the bottom screen")) / 2, 95, RGBA8(255, 255, 255, giveTransparence()), 15, "Edit your Pokemon in the bottom screen");
		sftd_draw_text(fontBold15, (400 - sftd_get_text_width(fontBold15, 15, "Tap SAVE when you're done")) / 2, 115, RGBA8(255, 255, 255, giveTransparence()), 15, "Tap SAVE when you're done");
	} else if (mode == ED_ITEMS) {
		int entry;
		int y = 0;
		sf2d_draw_texture(topMovesBG, 0, 0);
		for (int i = 0; i < 20; i++) {
			entry = 40 * additional2 + i;
			if (i == additional1) 
				sf2d_draw_texture(topSelectedMove, 2, y);
			else if (i + 20 == additional1) 
				sf2d_draw_texture(topSelectedMove, 200, y);
			sftd_draw_wtextf(fontBold9, 2, y, WHITE, 9, L"%d - %ls", entry, itemsSorted[entry]);
			sftd_draw_wtextf(fontBold9, 200, y, WHITE, 9, L"%d - %ls", entry + 20, itemsSorted[entry + 20]);
			y += (topSelectedMove->height + 1);
		}	
	} else if (mode == ED_MOVES) {
		int entry;
		int y = 0;
		sf2d_draw_texture(topMovesBG, 0, 0);
		for (int i = 0; i < 20; i++) {
			entry = 40 * additional2 + i;
			if (i == additional1) 
				sf2d_draw_texture(topSelectedMove, 2, y);
			else if (i + 20 == additional1) 
				sf2d_draw_texture(topSelectedMove, 200, y);
			sftd_draw_wtextf(fontBold9, 2, y, WHITE, 9, L"%d - %ls", entry, movesSorted[entry]);
			sftd_draw_wtextf(fontBold9, 200, y, WHITE, 9, L"%d - %ls", entry + 20, movesSorted[entry + 20]);
			y += (topSelectedMove->height + 1);
		}
	} else if (mode == ED_NATURES) {
		wchar_t* hor[] = {L"중립", L"-공", L"-방", L"-스피드", L"-특공", L"-특방"};
		wchar_t* ver[] = {L"+공", L"+방", L"+스피드", L"+특공", L"+특방"};
		
		sf2d_draw_texture(naturestx, 0, 0);
		for (int i = 0; i < 6; i++)
			sftd_draw_wtext(fontBold12, 66 * i + (66 - sftd_get_wtext_width(fontBold12, 12, hor[i])) / 2, 13, (i == 0) ? YELLOW : BLUE, 12, hor[i]);
		for (int i = 0; i < 5; i++)
			sftd_draw_wtext(fontBold12, (66 - sftd_get_wtext_width(fontBold12, 12, ver[i])) / 2, 53 + i * 40, BLUE, 12, ver[i]);
		
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (additional1 == i * 5 + j)
					sf2d_draw_texture(naturesButton, 66 + j * 66 + j, 40 + i * 40);
				sftd_draw_wtext(fontBold12, 66 + 66 * j + (66 - sftd_get_wtext_width(fontBold12, 12, natures[i * 5 + j])) / 2 + j, 53 + i * 40, (i == j) ? YELLOW : WHITE, 12, natures[i * 5 + j]);
			}
		}
	} else if (mode == ED_BALLS) {
		char* entries[] = {"Master Ball", "Ultra Ball", "Great Ball", "Poke Ball", "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball", "Repeat Ball", "Timer Ball", "Luxury Ball", "Premier Ball", "Dusk Ball", "Heal Ball", "Quick Ball", "Cherish Ball", "Fast Ball", "Level Ball", "Lure Ball", "Heavy Ball", "Love Ball", "Friend Ball", "Moon Ball", "Sport Ball", "Dream Ball", "Beast Ball", " ", " ", " ", " "};
		sf2d_draw_texture(ballsBG, 0, 0);
		
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 6; j++) {
				if (additional1 == i * 6 + j)
					sf2d_draw_texture(ballButton, j * 66 + j, i * 47 + i);
				sf2d_draw_texture_part(balls, 17 + 66 * j + j, 2 + i * 47, 32 * ((i * 6 + j + 1) % 8), 32 * ((i * 6 + j + 1) / 8), 32, 32);
				sftd_draw_text(fontBold9, 66 * j + (66 - sftd_get_text_width(fontBold9, 9, entries[i * 6 + j])) / 2 + j, 30 + i * 47 + i, WHITE, 9, entries[i * 6 + j]);
			}
		}
	} else if (mode == ED_HIDDENPOWER) {
		sf2d_draw_texture(hiddenPowerBG, 0, 0);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (additional1 == i * 4 + j)
					sf2d_draw_texture(hiddenPowerButton, j * 99 + j, + i * 59+ i);
				sf2d_draw_texture_part(typesSheet, 24 + 99 * j + j, 20 + 59 * i + i, 50 * (i * 4 + j + 1), 0, 50, 18); 
			}
		}
	} else if (mode == ED_FORMS) {
		char* entries[] = {"Default", "Alolan", "Default", "Rock Star", "Belle", "Pop Star", "Ph.D", "Libre", "Cosplay", "Default", "Original", "Hoenn", "Sinnoh", "Unova", "Kalos", "Alola", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "!", "?", "Normal", "Attack", "Defense", "Speed", "Plant", "Sandy", "Trash", "West Sea", "East Sea", "Default", "Heat", "Wash", "Fridge", "Fan", "Mow", "Altered", "Origin", "Land", "Sky", "Red-Striped", "Blue-Striped", "Spring", "Summer", "Autumn", "Winter", "Incarnate", "Therian", "Default", "White", "Black", "Ordinary", "Resolute", "Aria", "Pirouette", "Default", "Battle Bond", "Icy Snow", "Polar", "Tundra", "Continental", "Garden", "Elegant", "Meadow", "Modern", "Marine", "Archipelago", "High Plains", "Sandstorm", "River", "Monsoon", "Savanna", "Sun", "Ocean", "Jungle", "Fancy", "Poke Ball", "Red", "Yellow", "Orange", "Blue", "White", "Eternal Flower", "Natural", "Heart", "Star", "Diamond", "Debutante", "Matron", "Dandy", "La Reine", "Kabuki", "Pharaoh", "Average", "Small", "Large", "Super", "50%", "10%", "10%-PC", "50%-PC", "Confined", "Unbound", "Baile", "Pom-Pom", "Pa'u", "Sensu", "Midday", "Midnight", "Red", "Orange", "Yellow", "Green", "Blue", "Indigo", "Violet", "Default", "Original Color", "Default", "Spiky-Eared"};
		FormData *forms = getLegalFormData((u16)additional2, game);
		int numforms = forms->max - forms->min + 1;
		
		int rows, columns, width, height;
		sf2d_texture *button = NULL;
		if (numforms <= 16) {
			columns = 4; rows = 4;
			width = 99; height = 59;
			button = hiddenPowerButton;
			sf2d_draw_texture(hiddenPowerBG, 0, 0);
		} else {
			columns = 6; rows = 5;
			width = 66; height = 47;
			button = ballButton;
			sf2d_draw_texture(ballsBG, 0, 0);
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (additional1 == i * columns + j)
					sf2d_draw_texture(button, j * width + j, i * height + i);

				int form = i * columns + j;
				if (form < numforms) {
					char *str = entries[forms->stringNum + form];
					if (forms->min > 0)
						form++;
					if (form == 0 || forms->spriteNum == 0)
						sf2d_draw_texture_part(spritesSmall, (width - 34) / 2 + width * j + j, 2 * (height - 44) / 3 + i * height, 40 * (additional2 % 25) + 4, 30 * (additional2 / 25), 34, 30);
					else {
						int sprite = forms->spriteNum + form - 2;
						sf2d_draw_texture_part(alternativeSpritesSmall, (width - 34) / 2 + width * j + j, 2 * (height - 44) / 3 + i * height + i, 40 * (sprite % 6) + 4, 30 * (sprite / 6), 34, 30);
					}
					sftd_draw_text(fontBold9, width * j + (width - sftd_get_text_width(fontBold9, 9, str)) / 2 + j, (height * 5) / 7 + i * height + i, WHITE, 9, str);
				}
			}
		}
	} else if (mode == ED_HEX) {
		sf2d_draw_texture(hexBG, 0, 0);
		if (hax)
			sf2d_draw_rectangle(0, 0, 400, 240, RGBA8(255, 0, 0, 100));
		for (int rows = 0; rows < 15; rows++) {
			for (int columns = 0; columns < 16; columns++) {
				int byte = rows*16 + columns;
				if (additional1 == byte)
					sf2d_draw_texture(hexButton, columns*25, rows*15);
				sftd_draw_textf(fontBold11, 4 + 25*columns, 15*rows, (sector[byte][0]) ? WHITE : DS, 11, "%02hhX", pkmn[byte]);
				
				if (byte == 231) break;
			}
		}
		sftd_draw_textf(fontBold11, 4, 225, LIGHTBLUE, 11, "%s", descriptions[additional1]);
	}
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		if (mode != ED_HEX)
			printAnimatedBG(false);
		if (mode == ED_BASE || mode == ED_ITEMS || mode == ED_NATURES || mode == ED_BALLS || mode == ED_FORMS) {
			sf2d_draw_texture(editorBG, 0, 1);
			sf2d_draw_texture(editorBar, 0, 210);
			sf2d_draw_texture(hexIcon, 290, 1);
			
			sftd_draw_wtext(fontBold12, 27, 4, WHITE, 12, personal.species[n]);
			sf2d_draw_texture_part(balls, -2, -6, 32 * (getBall(pkmn) % 8), 32 * (getBall(pkmn) / 8), 32, 32);
			
			u16 t = getAlternativeSprite(pkmn, game);
			int ofs = movementOffsetSlow(3);
			if (t) {
				t -= 1;
				sf2d_draw_texture_part_scale_blend(alternativeSpritesSmall, 232, 32 - ofs, 40 * (t % 6) + 4, 30 * (t / 6), 34, 30, 2, 2, RGBA8(0x0, 0x0, 0x0, 100)); 
				sf2d_draw_texture_part_scale(alternativeSpritesSmall, 227, 27 - ofs, 40 * (t % 6) + 4, 30 * (t / 6), 34, 30, 2, 2);
			} else {
				sf2d_draw_texture_part_scale_blend(spritesSmall, 232, 32 - ofs, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30, 2, 2, RGBA8(0x0, 0x0, 0x0, 100));
				sf2d_draw_texture_part_scale(spritesSmall, 227, 27 - ofs, 40 * (n % 25) + 4, 30 * (n / 25), 34, 30, 2, 2);
			}
			
			if (getGender(pkmn) == 0)
				sf2d_draw_texture(male, 159, 6);
			else if (getGender(pkmn) == 1)
				sf2d_draw_texture(female, 161, 6);
			
			for (int i = 0; i < 3; i++) {
				sf2d_draw_texture(button, 206, 116 + i * 27);
				sftd_draw_wtext(fontBold12, 206 + (109 - sftd_get_wtext_width(fontBold12, 12, options[i])) / 2, 123 + i * 27, BLACK, 12, options[i]);
			}
			
			for (int i = 0; i < 9; i++)
				sftd_draw_wtext(fontBold12, 2, 29 + i * 20, LIGHTBLUE, 12, entries[i]);

			for (int i = 0; i < 7; i++)
				sf2d_draw_texture(setting, 180, 51 + i * 20);

			sf2d_draw_texture(minus, 180 - max - 6 - minus->width, 29);
			sf2d_draw_texture(plus, 180, 29);
			sf2d_draw_texture(minus, 180 - max - 6 - minus->width, 189);
			sf2d_draw_texture(plus, 180, 189);

			char* level = (char*)malloc(4 * sizeof(char));
			snprintf(level, 4, "%u", getLevel(pkmn));
			sftd_draw_text(fontBold12, 180 - max - 3 + (max - sftd_get_text_width(fontBold12, 12, level)) / 2, 29, WHITE, 12, level);
			free(level);
				
			sftd_draw_wtext(fontBold12, 178 - sftd_get_wtext_width(fontBold12, 12, natures[getNature(pkmn)]), 49, WHITE, 12, natures[getNature(pkmn)]);
			sftd_draw_wtext(fontBold12, 178 - sftd_get_wtext_width(fontBold12, 12, abilities[getAbility(pkmn)]), 69, WHITE, 12, abilities[getAbility(pkmn)]);
			sftd_draw_wtext(fontBold12, 178 - sftd_get_wtext_width(fontBold12, 12, items[getItem(pkmn)]), 89, WHITE, 12, items[getItem(pkmn)]);
			sftd_draw_wtext(fontBold12, 178 - sftd_get_wtext_width(fontBold12, 12, isShiny(pkmn) ? L"O" : L"X"), 109, WHITE, 12, isShiny(pkmn) ? L"O" : L"X");
			sftd_draw_wtext(fontBold12, 178 - sftd_get_wtext_width(fontBold12, 12, isInfected(pkmn) ? L"O" : L"X"), 129, WHITE, 12, isInfected(pkmn) ? L"O" : L"X");
			
			char* friendship = (char*)malloc(4 * sizeof(char));
			snprintf(friendship, 4, "%u", getFriendship(pkmn));
			sftd_draw_text(fontBold12, 180 - max - 3 + (max - sftd_get_text_width(fontBold12, 12, friendship)) / 2, 189, WHITE, 12, friendship);
			free(friendship);
			
			u32 nick[NICKNAMELENGTH*2];
			memset(nick, 0, NICKNAMELENGTH*2);
			getNickname(pkmn, nick);
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 178 - (sftd_get_wtext_width((isKor) ? unicodeKOR12 : unicodeJPN12, 12, (wchar_t*)nick)), 169, WHITE, 12, (wchar_t*)nick);

			u32 ot_name[NICKNAMELENGTH*2];
			memset(ot_name, 0, NICKNAMELENGTH*2);
			getOT(pkmn, ot_name);
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 178 - (sftd_get_wtext_width((isKor) ? unicodeKOR12 : unicodeJPN12, 12, (wchar_t*)ot_name)), 149, WHITE, 12, (wchar_t*)ot_name);
		}
		if (mode == ED_STATS || mode == ED_HIDDENPOWER) {
			sf2d_draw_texture(editorStatsBG, 0, 1);
			sf2d_draw_texture(editorBar, 0, 210);
			sf2d_draw_texture(setting, 291, 175);
			sftd_draw_wtext(fontBold12, 2, 28, LIGHTBLUE, 12, L"스탯");
			sftd_draw_wtext(fontBold12, 118, 28, DARKBLUE, 12, L"종족값");
			sftd_draw_wtext(fontBold12, 197, 28, DARKBLUE, 12, L"노력값");
			sftd_draw_wtext(fontBold12, 256, 28, DARKBLUE, 12, L"합계");
			sftd_draw_wtext(fontBold12, 2, 173, LIGHTBLUE, 12, L"숨겨진힘:");
			
			sftd_draw_wtext(fontBold12, 27, 4, WHITE, 12, personal.species[n]);
			sf2d_draw_texture_part(balls, -2, -6, 32 * (getBall(pkmn) % 8), 32 * (getBall(pkmn) / 8), 32, 32);
			
			if (getGender(pkmn) == 0)
				sf2d_draw_texture(male, 159, 6);
			else if (getGender(pkmn) == 1)
				sf2d_draw_texture(female, 161, 6);
			
			for (int i = 0; i < 6; i++)
				sftd_draw_wtext(fontBold12, 2, 49 + i * 20, LIGHTBLUE, 12, values[i]);

			char* tmp = (char*)malloc(4);
			for (int i = 0; i < 6; i++) {
				snprintf(tmp, 4, "%d", getIV(pkmn, lookup[i]));
				sftd_draw_text(fontBold12, 112 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 49 + i * 20, WHITE, 12, tmp);
				snprintf(tmp, 4, "%d", getEV(pkmn, lookup[i]));
				sftd_draw_text(fontBold12, 192 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 49 + i * 20, WHITE, 12, tmp);
				snprintf(tmp, 4, "%d", getStat(pkmn, lookup[i]));
				sftd_draw_text(fontBold12, 263 + (max - sftd_get_text_width(fontBold12, 12, tmp)) / 2, 49 + i * 20, WHITE, 12, tmp);
			}
			free(tmp);
			
			sftd_draw_wtext(fontBold12, 288 - sftd_get_wtext_width(fontBold12, 12, hpList[getHPType(pkmn)]), 173, WHITE, 12, hpList[getHPType(pkmn)]);	

			for (int i = 0; i < 6; i++) {
				sf2d_draw_texture(minus, 96, 49 + i * 20);
				sf2d_draw_texture(plus, 139, 49 + i * 20);
				sf2d_draw_texture(minus, 177, 49 + i * 20);
				sf2d_draw_texture(plus, 218, 49 + i * 20);
			}
		}
		if (mode == ED_MOVES) {
			sf2d_draw_texture(movesBottom, 0, 1);
			sf2d_draw_texture(movesBottom, 0, 2 + movesBottom->height);
			sf2d_draw_texture(editorBar, 0, 210);
			sftd_draw_wtext(fontBold12, 2, 5, LIGHTBLUE, 12, L"기술");
			sftd_draw_wtext(fontBold12, 2, 110, LIGHTBLUE, 12, L"배웠던 기술");
			
			for (int i = 0; i < 4; i++) {
				sftd_draw_wtext(fontBold12, 2, 28 + i * 20, (i == additional3) ? YELLOW : WHITE, 12, moves[getMove(pkmn, i)]);
				sftd_draw_wtext(fontBold12, 2, 132 + i * 20, (i == additional3 - 4) ? YELLOW: WHITE, 12, moves[getEggMove(pkmn, i)]);
				if (i == additional3)
					sf2d_draw_texture_rotate(subArrow, 198 - movementOffsetSlow(3), 33 + i * 20, 3.1415f);
				else if (i == additional3 - 4)
					sf2d_draw_texture_rotate(subArrow, 198 - movementOffsetSlow(3), 137 + i * 20, 3.1415f);
			}
		}
		if (mode == ED_HEX) {
			sf2d_draw_texture(mainMenuBottom, 0, 0);
			sf2d_draw_texture(blueTextBox, 165, 28);
			if (sector[additional1][0] && !(sector[additional1][1])) {
				sf2d_draw_texture(minusButton, 224, 31);
				sf2d_draw_texture(plusButton, 247, 31);
			}
			if (hax)
				sf2d_draw_rectangle(0, 0, 320, 240, RGBA8(255, 0, 0, 100));
			
			sftd_draw_textf(fontBold14, 50, 30, LIGHTBLUE, 14, "Selected byte:");
			sftd_draw_textf(fontBold14, 171, 30, WHITE, 14, "0x%02hhX", additional1);
			
			printfHexEditorInfo(pkmn, additional1);
		}

		if (mode != ED_HEX)
			sf2d_draw_texture(back, 280, 210);
		
		// apply masks
		if (mode == ED_ITEMS) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select an item with A in the top screen.")) / 2, 105, WHITE, 14, "Select an item with A in the top screen.");
		} else if (mode == ED_NATURES) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select a nature with A in the top screen.")) / 2, 105, WHITE, 14, "Select a nature with A in the top screen.");
		} else if (mode == ED_BALLS) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select a ball with A in the top screen.")) / 2, 105, WHITE, 14, "Select a ball with A in the top screen.");
		} else if (mode == ED_HIDDENPOWER) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select a HP type with A in the top screen.")) / 2, 105, WHITE, 14, "Select a HP type with A in the top screen.");
		} else if (mode == ED_FORMS) {
			sf2d_draw_rectangle(0, 0, 320, 240, MASKBLACK);
			sftd_draw_text(fontBold14, (320 - sftd_get_text_width(fontBold14, 14, "Select a form with A in the top screen.")) / 2, 105, WHITE, 14, "Select a form with A in the top screen.");
		}
		
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printPKBank(u8* bankbuf, u8* mainbuf, u8* pkmnbuf, int game, int currentEntry, int saveBox, int bankBox, bool isBufferized, bool isSeen) {
	int x, y;
	int pointer[2] = {0, 0};
	char* page = (char*)malloc(10 * sizeof(char));
	bool isKor = (pkmnbuf[0xE3] == 0x08) ? true : false;
	
	u8* pkmn = (u8*)malloc(PKMNLENGTH * sizeof(u8));
	if (currentEntry < 30)
		memcpy(pkmn, &bankbuf[bankBox * 30 * PKMNLENGTH + currentEntry * PKMNLENGTH], PKMNLENGTH);
	else
		getPkmn(mainbuf, saveBox, currentEntry - 30, pkmn, game);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		if (isSeen) {
			infoViewer(pkmnbuf, game);
		} else {
			printAnimatedBG(true);
			sf2d_draw_texture(bankTop, 34, 5);
			snprintf(page, 10, "Bank %d", bankBox + 1);
			sftd_draw_text(fontBold12, 55 + (178 - sftd_get_text_width(fontBold12, 12, page)) / 2, 9, WHITE, 12, page);

			if (getPokedexNumber(pkmn) > 0 && getPokedexNumber(pkmn) < 822) {
				u16 tempspecies = getFormSpeciesNumber(pkmn);
				u8 type1 = 0, type2 = 0;

				if (tempspecies == 493 || tempspecies == 773) {
					type1 = getForm(pkmn);
					if (game >= GAME_DIAMOND && game <= GAME_SS && type1 > 9)
						type1--;
					type2 = type1;
				} else {
					type1 = personal.pkmData[tempspecies][0x6];
					type2 = personal.pkmData[tempspecies][0x7];
				}
				
				sf2d_draw_texture_part(typesSheet, 273, 120, 50 * type1, 0, 50, 18); 
				if (type1 != type2)
					sf2d_draw_texture_part(typesSheet, 325, 120, 50 * type2, 0, 50, 18); 
				
				u32 nick[NICKNAMELENGTH*2];
				memset(nick, 0, NICKNAMELENGTH*2);
				getNickname(pkmn, nick);
				sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 273, 69, WHITE, 12, (wchar_t*)nick);
				
				char* level = (char*)malloc(8 * sizeof(char));
				snprintf(level, 8, "Lv.%u", getLevel(pkmn));
				sftd_draw_text(fontBold12, 372 - sftd_get_text_width(fontBold12, 12, level), 86, WHITE, 12, level);
				
				if (getGender(pkmn) == 0)
					sf2d_draw_texture(male, 358 - sftd_get_text_width(fontBold12, 12, level), 86);
				else if (getGender(pkmn) == 1)
					sf2d_draw_texture(female, 360 - sftd_get_text_width(fontBold12, 12, level), 86);
				if (isShiny(pkmn))
					sf2d_draw_texture(shinyStar, 360 - sftd_get_text_width(fontBold12, 12, level) - 14, 88);
				
				free(level);
				
				u32 ot_name[NICKNAMELENGTH*2];
				memset(ot_name, 0, NICKNAMELENGTH*2);
				getOT(pkmn, ot_name);
				sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, 273, 146, WHITE, 12, (wchar_t*)ot_name);

				char* otid = (char*)malloc(12 * sizeof(char));
				snprintf(otid, 12, "ID. %u", getOTID(pkmn));
				sftd_draw_text(fontBold12, 372 - sftd_get_text_width(fontBold12, 12, otid), 163, WHITE, 12, otid);
				free(otid);
				
				sftd_draw_wtext(fontBold12, 273, 104, WHITE, 12, personal.species[getPokedexNumber(pkmn)]);
			}
			y = 45;
			for (int i = 0; i < 5; i++) {
				x = 44;
				for (int j = 0; j < 6; j++) {
					memcpy(pkmn, &bankbuf[bankBox * 30 * PKMNLENGTH + (i * 6 + j) * PKMNLENGTH], PKMNLENGTH);
					u16 n = getPokedexNumber(pkmn);
					if (n)
						printElement(mainbuf, pkmn, GAME_SUN, n, x, y);

					if (currentEntry == (i * 6 + j)) {
						pointer[0] = x + 18;
						pointer[1] = y - 8;
					}
					x += 34;
				}
				y += 30;
			}
			
			if (currentEntry < 30) {
				u16 n = getPokedexNumber(pkmnbuf);
				if (n) printElementBlend(pkmnbuf, GAME_SUN, n, pointer[0] - 14, pointer[1] + 8, RGBA8(0x0, 0x0, 0x0, 100));
				if (n) printElement(mainbuf, pkmnbuf, GAME_SUN, n, pointer[0] - 18, pointer[1] + 3);
				sf2d_draw_texture(selector, pointer[0], pointer[1] - 2 - ((!isBufferized) ? movementOffsetSlow(3) : 0));
			}
		}		
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(boxView, 0, 0);
		sf2d_draw_texture(editorBar, 0, 210);
		snprintf(page, 10, "Save %d", saveBox + 1);
		sftd_draw_text(fontBold12, 12 + (178 - sftd_get_text_width(fontBold12, 12, page)) / 2, 19, WHITE, 12, page);
		sf2d_draw_texture(left, 7, 17);
		sf2d_draw_texture(right, 185, 17);
		sf2d_draw_texture(transferButton, 242, 5);
		sf2d_draw_texture(back, 280, 210);
		sf2d_draw_texture(button, 208, 43);
		sf2d_draw_texture(button, 208, 70);
		sf2d_draw_texture(button, 208, 97);
		sftd_draw_text(fontBold12, 208 + (109 - sftd_get_text_width(fontBold12, 12, "Y: VIEW")) / 2, 50, BLACK, 12, "Y: VIEW");
		sftd_draw_text(fontBold12, 208 + (109 - sftd_get_text_width(fontBold12, 12, "CLEAR BOX")) / 2, 77, BLACK, 12, "CLEAR BOX");
		sftd_draw_text(fontBold12, 208 + (109 - sftd_get_text_width(fontBold12, 12, "RELEASE")) / 2, 104, BLACK, 12, "RELEASE");
				
		y = 45;
		for (int i = 0; i < 5; i++) {
			x = 4;
			for (int j = 0; j < 6; j++) {
				getPkmn(mainbuf, saveBox, i*6+j, pkmn, game);
				u16 n = getPokedexNumber(pkmn);
				if (n)
					printElement(mainbuf, pkmn, game, n, x, y);

				if ((currentEntry - 30) == (i * 6 + j)) {
					pointer[0] = x + 18;
					pointer[1] = y - 8;
				}
				x += 34;
			}
			y += 30;
		}
		
		if (currentEntry > 29) {
			if (!isSeen) {
				u16 n = getPokedexNumber(pkmnbuf);
				if (n) printElementBlend(pkmnbuf, GAME_SUN, n, pointer[0] - 14, pointer[1] + 8, RGBA8(0x0, 0x0, 0x0, 100));
				if (n) printElement(mainbuf, pkmnbuf, GAME_SUN, n, pointer[0] - 18, pointer[1] + 3);
				sf2d_draw_texture(selector, pointer[0], pointer[1] - 2 - ((!isBufferized) ? movementOffsetSlow(3) : 0));
			} else
				sf2d_draw_texture(selector, pointer[0], pointer[1] - 2);
		}
		
		if (isSeen)
			sf2d_draw_rectangle(0, -30, 320, 240, MASKBLACK);
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(pkmn);
	free(page);
}

void printMassInjector(int currentEntry) {
	int y = 41;
	char* entries[] = {"XD Collection", "Colosseum Collection", "10th Anniversary Collection", "N's Collection", "Entree Forest Collection", "Dream Radar Collection", "Living Dex", "Oblivia Deoxys Collection", "Pokemon Ranch Collection", "KOR Events Collection"};
	int boxes[] = {3, 2, 1, 1, 1, 1, 27, 1, 1, 1};
	
	char* message = (char*)malloc(30 * sizeof(char));
	snprintf(message, 30, "%d boxes will be replaced.", boxes[currentEntry]);
	
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(cleanTop, 0, 0);
		printTitle("Mass Injector");
		
		for (int i = 0; i < 5; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 18, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 18, y);
			
			sftd_draw_text(fontBold12, 18 + (182 - sftd_get_text_width(fontBold12, 12, entries[i])) / 2, y + 10, (i == currentEntry) ? DARKBLUE : YELLOW, 12, entries[i]);
			
			y += 37;
		}
		
		y = 41;
		for (int i = 5; i < 10; i++) {
			if (i == currentEntry)
				sf2d_draw_texture(eventMenuTopBarSelected, 200, y);
			else
				sf2d_draw_texture(eventMenuTopBar, 200, y);
			
			sftd_draw_text(fontBold12, 200 + (182 - sftd_get_text_width(fontBold12, 12, entries[i])) / 2, y + 10, (i == currentEntry) ? DARKBLUE : YELLOW, 12, entries[i]);
			
			y += 37;
		}
	pksm_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);
		sftd_draw_text(fontBold12, (320 - sftd_get_text_width(fontBold12, 12, message)) / 2, 12, LIGHTBLUE, 12, message);
		printBottomIndications("Press START to inject, B to exit.");
	pksm_end_frame();
	sf2d_swapbuffers();
	
	free(message);
}

void printSettings(int box) {
	char *menu[] = {"Bank Size:                   ", "Backup Save", "Backup Bank"};
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawMenuTop(0);
	pksm_end_frame();
	
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(mainMenuBottom, 0, 0);

		for (int i = 0; i < 3; i++) {
			sf2d_draw_texture(menuBar, (320 - menuBar->width) / 2, 60 + i * (menuBar->height));
			sftd_draw_text(fontBold15, (320 - sftd_get_text_width(fontBold15, 15, menu[i])) / 2 - 4, 55 + (menuBar->height - 18) / 2 + i * (menuBar->height), DARKBLUE, 15, menu[i]);
		}
		
		sf2d_draw_texture(miniBox, 189, 64);
		sf2d_draw_texture(minusButton, 169, 65);
		sf2d_draw_texture(plusButton, 228, 65);
		
		char* size = (char*)malloc(5);
		snprintf(size, 5, "%d", box);
		sftd_draw_text(fontBold11, 189 + (36 - (sftd_get_text_width(fontBold11, 11, size))) / 2, 68, WHITE, 11, size);
		free(size);

		sftd_draw_text(fontBold9, 85, 225, LIGHTBLUE, 9, "Tap the number to change size.");
	pksm_end_frame();
	sf2d_swapbuffers();
}

void printfHexEditorInfo(u8* pkmn, int byte) {
	int y = 70, x = 8, xribbon = 90;
	bool isKor = (pkmn[0xE3] == 0x08) ? true : false;
	u32 string[NICKNAMELENGTH*2];
	memset(string, 0, NICKNAMELENGTH*2);
	
	switch (byte) {
		case 0x08 :
		case 0x09 :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Species: #%d - %ls", getPokedexNumber(pkmn), personal.species[getPokedexNumber(pkmn)]);
			break;
		case 0x0A :
		case 0x0B :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Item: #%d - %s", getItem(pkmn), items[getItem(pkmn)]);
			break;
		case 0x0C :
		case 0x0D :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "TID: %d", getOTID(pkmn));
			break;
		case 0x0E :
		case 0x0F :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "SID: %d", getSOTID(pkmn));
			break;
		case 0x14 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Ability: %s", abilities[getAbility(pkmn)]);
			break;
		case 0x1C :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Nature: %s", natures[getNature(pkmn)]);
			break;
		case 0x1E : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "HP EV: %d", pkmn[byte]);
			break;
		case 0x1F : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "ATK EV: %d", pkmn[byte]);
			break;
		case 0x20 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "DEF EV: %d", pkmn[byte]);
			break;	
		case 0x21 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "SPE EV: %d", pkmn[byte]);
			break;
		case 0x22 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "SPA EV: %d", pkmn[byte]);
			break;	
		case 0x23 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "SPD EV: %d", pkmn[byte]);
			break;	
		case 0x24 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Cool: %d", pkmn[byte]);
			break;	
		case 0x25 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Beauty: %d", pkmn[byte]);
			break;	
		case 0x26 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Cute: %d", pkmn[byte]);
			break;	
		case 0x27 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Smart: %d", pkmn[byte]);
			break;	
		case 0x28 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Tough: %d", pkmn[byte]);
			break;	
		case 0x29 : 
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Contest Value Sheen: %d", pkmn[byte]);
			break;
		case 0x30 : {
			char* entries[] = {"Champion Kalos", "Champion G3 Hoenn", "Champion Sinnoh", "Best Friends", "Training", "Skillful Battler", "Battler Expert", "Effort"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 0, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x31 : {
			char* entries[] = {"Alert", "Shock", "Downcast", "Careless", "Relax", "Snooze", "Smile", "Gorgeous"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 1, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x32 : {
			char* entries[] = {"Royal", "Gorgeous Royal", "Artist", "Footprint", "Record", "Legend", "Country", "National"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 2, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x33 : {
			char* entries[] = {"Earth", "World", "Classic", "Premier", "Event", "Birthday", "Special", "Souvenir"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 3, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x34 : {
			char* entries[] = {"Wishing", "Champion Battle", "Champion Regional", "Champion National", "Champion World", "-", "-", "Champion G6 Hoenn"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 4, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x35 : {
			char* entries[] = {"Contest Star", "Master Coolness", "Master Beauty", "Master Cuteness", "Master Cleverness", "Master Toughness", "Champion Alola", "Battle Royale"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 5, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x36 : {
			char* entries[] = {"Battle Tree Great", "Battle Tree Master", "-", "-", "-", "-", "-", "-"};
			for (int i = 0; i < 8; i++) {
				sftd_draw_text(fontBold12, xribbon + 27, y + 17*i, LIGHTBLUE, 12, entries[i]);
				sf2d_draw_rectangle(xribbon, y + 17*i, 13, 13, (getRibbons(pkmn, 6, i)) ? BUTTONGREEN : BUTTONRED);
			}
			break;
		}
		case 0x40 :
		case 0x41 :
		case 0x42 :
		case 0x43 :
		case 0x44 :
		case 0x45 :
		case 0x46 :
		case 0x47 :
		case 0x48 :
		case 0x49 :
		case 0x4A :
		case 0x4B :
		case 0x4C :
		case 0x4D :
		case 0x4E :
		case 0x4F :
		case 0x50 :
		case 0x51 :
		case 0x52 :
		case 0x53 :
		case 0x54 :
		case 0x55 :
		case 0x56 :
		case 0x57 :
			getNickname(pkmn, string);
			sftd_draw_text(fontBold12, x, y, LIGHTBLUE, 12, "Nickname: ");
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, x + sftd_get_wtext_width(fontBold12, 12, L"Nickname: "), y, LIGHTBLUE, 12, (wchar_t*)string);
			break;
		case 0x5A :
		case 0x5B :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Move 1: #%d - %s", getMove(pkmn, 0), moves[getMove(pkmn, 0)]);
			break;
		case 0x5C :
		case 0x5D :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Move 2: #%d - %s", getMove(pkmn, 1), moves[getMove(pkmn, 1)]);
			break;
		case 0x5E :
		case 0x5F :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Move 3: #%d - %s", getMove(pkmn, 2), moves[getMove(pkmn, 2)]);
			break;
		case 0x60 :
		case 0x61 :
			sftd_draw_wtextf(fontBold12, x, y, LIGHTBLUE, 12, L"Move 4: #%d - %s", getMove(pkmn, 3), moves[getMove(pkmn, 3)]);
			break;
		case 0x62 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 1 PP: %d", pkmn[byte]);
			break;
		case 0x63 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 2 PP: %d", pkmn[byte]);
			break;
		case 0x64 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 3 PP: %d", pkmn[byte]);
			break;
		case 0x65 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 4 PP: %d", pkmn[byte]);
			break;
		case 0x66 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 1 PP Up: %d", pkmn[byte]);
			break;
		case 0x67 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 2 PP Up: %d", pkmn[byte]);
			break;
		case 0x68 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 3 PP Up: %d", pkmn[byte]);
			break;
		case 0x69 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Move 4 PP Up: %d", pkmn[byte]);
			break;
		case 0x6A :
		case 0x6B :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Relearn Move 1: #%d - %s", getEggMove(pkmn, 0), moves[getEggMove(pkmn, 0)]);
			break;
		case 0x6C :
		case 0x6D :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Relearn Move 2: #%d - %s", getEggMove(pkmn, 1), moves[getEggMove(pkmn, 1)]);
			break;
		case 0x6E :
		case 0x6F :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Relearn Move 3: #%d - %s", getEggMove(pkmn, 2), moves[getEggMove(pkmn, 2)]);
			break;
		case 0x70 :
		case 0x71 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Relearn Move 4: #%d - %s", getEggMove(pkmn, 3), moves[getEggMove(pkmn, 3)]);
			break;
		case 0x78 :
		case 0x79 :
		case 0x7A :
		case 0x7B :
		case 0x7C :
		case 0x7D :
		case 0x7E :
		case 0x7F :
		case 0x80 :
		case 0x81 :
		case 0x82 :
		case 0x83 :
		case 0x84 :
		case 0x85 :
		case 0x86 :
		case 0x87 :
		case 0x88 :
		case 0x89 :
		case 0x8A :
		case 0x8B :	
		case 0x8C :
		case 0x8D :
		case 0x8E :
		case 0x8F :	
			getHTName(pkmn, string);
			sftd_draw_text(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer name: ");
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, x + sftd_get_text_width(fontBold12, 12, "Held Trainer name: "), y, LIGHTBLUE, 12, (wchar_t*)string);
			break;
		case 0xA2 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Friendship: %d", pkmn[byte]);
			break;
		case 0xA3 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Affection: %d", pkmn[byte]);
			break;
		case 0xA4 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Intensity: %d", pkmn[byte]);
			break;
		case 0xA5 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Memory: %d", pkmn[byte]);
			break;	
		case 0xA6 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Held Trainer Feeling: %d", pkmn[byte]);
			break;
		case 0xAE :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Fullness: %d", pkmn[byte]);
			break;	
		case 0xAF :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Enjoyment: %d", pkmn[byte]);
			break;
		case 0xB0 :
		case 0xB1 :
		case 0xB2 :
		case 0xB3 :
		case 0xB4 :
		case 0xB5 :
		case 0xB6 :
		case 0xB7 :
		case 0xB8 :
		case 0xB9 :
		case 0xBA :
		case 0xBB :
		case 0xBC :
		case 0xBD :
		case 0xBE :
		case 0xBF :
		case 0xC0 :
		case 0xC1 :
		case 0xC2 :
		case 0xC3 :
		case 0xC4 :
		case 0xC5 :
		case 0xC6 :
		case 0xC7 :
			getOT(pkmn, string);
			sftd_draw_text(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Name: ");
			sftd_draw_wtext((isKor) ? unicodeKOR12 : unicodeJPN12, x + sftd_get_text_width(fontBold12, 12, "Original Trainer Name: "), y, LIGHTBLUE, 12, (wchar_t*)string);
			break;
		case 0xCA :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Friendship: %d", pkmn[byte]);
			break;
		case 0xCB :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Affection: %d", pkmn[byte]);
			break;
		case 0xCC :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Intensity: %d", pkmn[byte]);
			break;
		case 0xCD :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Memory: %d", pkmn[byte]);
			break;	
		case 0xD0 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Original Trainer Feeling: %d", pkmn[byte]);
			break;
		case 0xD1 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Egg Year: %d", 2000 + pkmn[byte]);
			break;
		case 0xD2 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Egg Month: %d", pkmn[byte]);
			break;
		case 0xD3 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Egg Day: %d", pkmn[byte]);
			break;
		case 0xD4 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Met Year: %d", 2000 + pkmn[byte]);
			break;
		case 0xD5 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Met Month: %d", pkmn[byte]);
			break;
		case 0xD6 :
			sftd_draw_textf(fontBold12, x, y, LIGHTBLUE, 12, "Met Day: %d", pkmn[byte]);
			break;
	}
}