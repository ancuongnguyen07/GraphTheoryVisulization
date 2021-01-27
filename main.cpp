#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string.h>
#include <iostream>
#include "math.h"
#include "algorithms.h"
#include <stdlib.h>

#define NUMOFINFOBOXES 4
#define WINDOWWIDTH  1000
#define WINDOWHEIGHT 720
#define SCREENWIDTH 1280
#define SCREENHEIGHT 720
#define NUMOFFUNCTIONICON 6
#define GAPBTICONS 20
#define UNITPIXEL 5
#define MAXDEMO 10
#define NUMOFDEMOS 10
#define CHARSIZEDEMOBUTTON 8
#define DEMOBUTTONGAP 30
#define MAXNODE 10
#define RADIUS 30
#define CHARSIZENODE 12
#define TITLEMATRIXBOXSIZE 10
#define CHARSIZEMATRIXTEXT 10
#define CURVEDSEGMENT 100
#define DISTANCEBEZIERCURVE 150
#define TEXTINPUTWIDTH 400
#define TEXTINPUTHEIGHT 180
#define TEXTINPUTFIELDWIDTH 300
#define TEXTINPUTFIELDHEIGHT 50
#define CHARSIZEINPUTBOX 12

using namespace sf;
using namespace std;

// icon ordered set in img file
enum ICON{
    ADD,UPLOAD,DOWNLOAD,EDIT,ARROW,DELETE
};

enum DEMOS{
    DFSf,BFSf,LIENTHONG,NGANNHAT,TRU,CAU,THAT,HAMILTON,EULER,TOPO
};

char DEMO[NUMOFDEMOS][30]={
    "DFS","BFS","LIENTHONG","NGANNHAT","TRU","CAU","THAT","HAMILTON","EULER","TOPO",
};

const int MYICONSET[]={ADD,ARROW,DELETE,EDIT,UPLOAD,DOWNLOAD}; // icon order to show
const int MYDEMOSET[]={DFSf,BFSf,LIENTHONG,NGANNHAT,TRU,CAU,THAT,HAMILTON,EULER,TOPO};
const Vector2u ICONSIZE(45,45);
const Vector2f ICONSETSTART(25,25);
const Vector2f SHADESIZE(6,6);
const Vector2f REVERSEDSHADESIZE(-6,-6);
const Vector2f BOXSIZE(50,50);
const Vector2f MATRIXBOXSIZE(250,300);
const Vector2f MATRIXBOXSTART(WINDOWWIDTH-MATRIXBOXSIZE.x-SHADESIZE.x-3,SHADESIZE.y);
const Vector2f DEMOBOXSIZE(250,370); // size of demo frame containing demo buttons
const Vector2f DEMOBOXSTART(WINDOWWIDTH-DEMOBOXSIZE.x-SHADESIZE.x-3,WINDOWHEIGHT-SHADESIZE.y-DEMOBOXSIZE.y-10);
const Vector2f DEMOBUTTONSIZE(80,30);
const int xDBT=DEMOBOXSTART.x+(DEMOBOXSIZE.x-((DEMOBUTTONSIZE.x+SHADESIZE.x)*2+DEMOBUTTONGAP))/2;
const int yDBT=DEMOBOXSTART.y+(DEMOBOXSIZE.y-((DEMOBUTTONSIZE.y+SHADESIZE.y)*NUMOFDEMOS/2+DEMOBUTTONGAP*(NUMOFDEMOS/2-1)))/2;
const Vector2f DEMOBUTTONSTART(xDBT,yDBT);
//const int xRESULTBOXSIZE=WINDOWWIDTH-DEMOBOXSIZE.x-SHADESIZE.x;
const Vector2f RESULTBOXSIZE(700,80);
const Vector2f RESULTBOXSTART(6,WINDOWHEIGHT-RESULTBOXSIZE.y-SHADESIZE.y-8);
const Vector2f TEXTINPUTBOXSIZE(TEXTINPUTFIELDWIDTH,TEXTINPUTFIELDHEIGHT);
const Vector2f TEXTINPUTFRAMESIZE(TEXTINPUTWIDTH,TEXTINPUTHEIGHT); // A BOX CONTAINS INPUT BOX
const Vector2f TEXTINPUTFRAMESTART(WINDOWWIDTH/2-TEXTINPUTWIDTH/2,WINDOWHEIGHT/2-TEXTINPUTHEIGHT/2);
const Vector2f OPTIONBOXSIZE(80,25);
const Vector2f ERRORBOXSIZE(500,150);
const Vector2f ERRORBOXSTART(WINDOWWIDTH/2-ERRORBOXSIZE.x/2,WINDOWHEIGHT/2-ERRORBOXSIZE.y/2);

const Color BOXCOLOR(176,253,182);
const Color SHADECOLOR=Color::Black;
const Color BACKGROUNDCOLOR(251,165,146);
const Color SELECTBOXCOLOR(112,125,250);
const Color ARROWLINECOLOR=BOXCOLOR;
const Color NODECOLOR=BOXCOLOR;
const Color NODEOUTLINECOLOR=SHADECOLOR;
const Color TEXTCOLOR=Color::Black;
string TXT=".txt";
string DAT=".dat";


Font myFont;


class IconSet : public Drawable, public Transformable{
    private:
        VertexArray m_vertices; // array of icons
        RectangleShape containBox;
        RectangleShape shadeBox;
        Texture m_iconset;

        virtual void draw(RenderTarget& target, RenderStates states) const{
            states.transform*=getTransform(); // apply transform
            states.texture=&m_iconset; // apply the tileset textture
            target.draw(shadeBox);
            target.draw(containBox);
            target.draw(m_vertices,states);
        }

        void getIcon(Vertex* quad,int x_texture,Vector2f start,Vector2u iconSize){
            quad[0].position=Vector2f(start.x,start.y);
            quad[1].position=Vector2f(start.x+iconSize.x,start.y);
            quad[2].position=Vector2f(start.x+iconSize.x,start.y+iconSize.y);
            quad[3].position=Vector2f(start.x,start.y+iconSize.y);

            quad[0].texCoords=Vector2f(x_texture,0);
            quad[1].texCoords=Vector2f(x_texture+iconSize.x,0);
            quad[2].texCoords=Vector2f(x_texture+iconSize.x,iconSize.y);
            quad[3].texCoords=Vector2f(x_texture,iconSize.y);
        }

        void getOriginalIcons(const int* icons,unsigned int numOfIcon,Vector2f start,Vector2u iconSize,unsigned int gap){
            int y_start=start.y;
            for (unsigned int i=0;i<numOfIcon;i++){
                Vertex* quad=&m_vertices[i*4];
                int x_texture=iconSize.x*icons[i];  // x_start coordinate in texture (img file)

                /*
                --------------------QUAD DIAGRAM:
                    0--------------->1
                    |                |
                    |                |
                    |                |
                    |                |
                    |                |
                    3<---------------2
                */
                getIcon(quad,x_texture,Vector2f(start.x,y_start),iconSize);
                y_start+=iconSize.y+gap;   // y coordinate in vertexArray
            }
        }

        void loadContainBox(Vector2u iconSize,Vector2f iconStart, unsigned int gap, unsigned numOfIcon,
                            unsigned int thickness,Color boxColor, Color shadeColor){
            containBox.setPosition(iconStart.x-gap,iconStart.y-gap);
            containBox.setFillColor(boxColor);
            containBox.setSize(Vector2f(iconSize.x+2*gap,
                                        (iconSize.y+gap)*numOfIcon+gap));
            containBox.setOutlineColor(shadeColor);
            containBox.setOutlineThickness(thickness);
        }

        void loadShadeBox(unsigned int gap,Color shadeColor){
            shadeBox.setPosition(containBox.getPosition());
            shadeBox.move(SHADESIZE.x,SHADESIZE.y);
            shadeBox.setSize(containBox.getSize());
            shadeBox.setFillColor(shadeColor);
        }

    public:
        bool load(const string& iconsetTitle, Vector2u iconSize, const int* icons,
                  unsigned int width, unsigned int height, Vector2f start, unsigned int gap,
                  Color colorBox, Color shadeColor=Color::Black, float thickness=2.5){
            if (!m_iconset.loadFromFile(iconsetTitle)) return false;
            m_vertices.setPrimitiveType(Quads);
            m_vertices.resize((width*height)*4);
            getOriginalIcons(icons,height,start,iconSize,gap);
            loadContainBox(iconSize,start,gap,height,thickness,colorBox,shadeColor);
            loadShadeBox(gap,shadeColor);
            return true;
        }


        VertexArray getVertices(){
            return m_vertices;
        }

        Vector2f getSize(){
            float x=shadeBox.getSize().x-containBox.getPosition().x;
            float y=shadeBox.getSize().y-containBox.getPosition().y;
            return Vector2f(x,y);
        }

        Vector2f getPos(){
            return containBox.getPosition();
        }

        RectangleShape getBox(){
            return this->containBox;
        }

};

class InfoBox: public Drawable, public Transformable{
    private:
        Text title;
        RectangleShape containBox;
        RectangleShape shadeBox;
        Text content;

        void loadContainBox(Vector2f start,Color boxColor,Vector2f boxSize,
                            Color outline, unsigned int thickness){
            containBox.setPosition(start);
            containBox.setFillColor(boxColor);
            containBox.setSize(boxSize);
            containBox.setOutlineColor(outline);
            containBox.setOutlineThickness(thickness);
        }

        void loadTitle(char* intro,Vector2f start,Color textColor,
                       unsigned int charSize,Font font=myFont){
            title.setString(intro);
            title.setFont(font);
            title.setCharacterSize(charSize);
            title.setPosition(start);
            title.setFillColor(textColor);

        }

        void loadText(string text,Vector2f start,int charSize,Color color=Color::Black){
            content.setString(text);
            content.setPosition(start);
            content.setCharacterSize(charSize);
            content.setFillColor(color);
        }

        void loadShadeBox(Color shadeColor){
            shadeBox.setPosition(containBox.getPosition());
            shadeBox.setSize(containBox.getSize());
            shadeBox.move(SHADESIZE.x,SHADESIZE.y);
            shadeBox.setFillColor(shadeColor);
        }

        virtual void draw(RenderTarget& target, RenderStates states) const{
            states.transform*=getTransform(); // apply transform
            target.draw(shadeBox);
            target.draw(containBox);
            //target.draw(title);
        }
    public:

        void setSize(Vector2f size){
            containBox.setSize(size);
        }

        void setSize(float x,float y){
            containBox.setSize(Vector2f(x,y));
        }

        void load(Font font,char* intro,Vector2f start,Vector2f boxSize,Color boxColor,unsigned int charSize=TITLEMATRIXBOXSIZE,
                  Color shadeColor=Color::Black,Color outlineColor=Color::Black,Color textColor=Color::Black,
                    unsigned int thickness=2.5){
            loadContainBox(start,boxColor,boxSize,outlineColor,thickness);
            loadShadeBox(shadeColor);
            unsigned int textTitleSize=charSize*strlen(intro);
            Vector2f textTitleStart(start.x+boxSize.x/2-textTitleSize/2,start.y+charSize);
            loadTitle(intro,textTitleStart,textColor,charSize,font);
            unsigned int contentCharSize=charSize;
            string t="";
            Vector2f contentStart(start.x+contentCharSize,textTitleStart.y+charSize+contentCharSize);
            loadText(t,contentStart,contentCharSize,textColor);
        }

        void reloadTitle(string text){ // reload when change the length of the title
            title.setString(text);
            unsigned int textSize=title.getCharacterSize()*text.length();
            unsigned int charSize=title.getCharacterSize();
            Vector2f boxSize=containBox.getSize();
            Vector2f start=containBox.getPosition();
            Vector2f textStart(start.x+boxSize.x/2-textSize/2,start.y+charSize);
            title.setPosition(textStart);
        }

        void reloadContent(string text){
            content.setString(text);
        }

        Text getContent(){return content;}
        RectangleShape getContainBox(){return containBox;}
        RectangleShape getShadeBox() {return shadeBox;}
        Text getTitle(){return title;}

        void setColorContainBox(Color colorBox){
            containBox.setFillColor(colorBox);
        }

        void setShadeBoxPos(Vector2f newPos){
            shadeBox.setPosition(newPos);
        }

        void moveShadeBox(Vector2f offset){
            shadeBox.move(offset);
        }

        void setShadeBoxSize(Vector2f newSize){
            shadeBox.setSize(newSize);
        }

        void setTitle(string text){
            title.setString(text);
        }

        void loadSelectedBox(){
            Vector2f temp=shadeBox.getSize();
            containBox.setFillColor(SELECTBOXCOLOR);
            shadeBox.setPosition(containBox.getPosition());
            shadeBox.move(REVERSEDSHADESIZE);
            shadeBox.setSize(Vector2f(temp.x+SHADESIZE.x+2,temp.y+SHADESIZE.y+2));
        }

        void loadUnselectedBox(){
            containBox.setFillColor(BACKGROUNDCOLOR);
            shadeBox.setPosition(containBox.getPosition());
            shadeBox.setSize(containBox.getSize());
            shadeBox.move(SHADESIZE);
        }
};
void drawInfoBox(RenderWindow*,InfoBox*);

class Circle: public Drawable, public Transformable{
    private:
        void loadTitle(string index,Vector2f start,Font font=myFont,
                       Color textColor=TEXTCOLOR,int charSize=CHARSIZENODE){
            title.setFont(font);
            title.setString(index);
            title.setFillColor(textColor);
            title.setCharacterSize(charSize);
            title.setPosition(start);
        }
        void loadShape(Vector2f start,int radius,Color BGColor,Color outlineColor,int thickness){
            shape.setFillColor(BGColor);
            shape.setPosition(start);
            shape.setRadius(radius);
            shape.setOutlineColor(outlineColor);
            shape.setOutlineThickness(thickness);
        }
        virtual void draw(RenderTarget& target, RenderStates states) const{
            states.transform*=getTransform(); // apply transform
            target.draw(shape);
            //target.draw(title);
        }
    public:
        Text title;
        CircleShape shape;
        void loadCircle(Vector2f start,int radius,string index,
                        Color outlineColor=Color::Black,int thickness=2.5,
                        Font font=myFont,Color BGColor=BOXCOLOR,
                       Color textColor=TEXTCOLOR,int charSize=CHARSIZENODE){
            loadShape(start,radius,BGColor,outlineColor,thickness);
            Vector2f titleStart=Vector2f(start.x+radius-charSize/2,start.y+radius-charSize/2);
            loadTitle(index,titleStart,font,textColor,charSize);
       }
       Text getTitle(){return title;}
       Vector2f getCenter(){
            Vector2f temp=shape.getPosition();
            return Vector2f(temp.x+RADIUS,temp.y+RADIUS);
       }
};

string convertItoS(int);

class CirclesList{
    private:


    public:
        //Circle* circles[MAXNODE];
        Circle *circles[MAXNODE];
        int n; // so luong phan tu
        CirclesList(){this->n=0;}
        bool isFull(){
            return n==MAXNODE;
        }
        bool isEmpty(){
            return n==0;
        }

        int getCircle(int index,Circle *result){
            if (index>=n || index<0) return 0;
            *result= *(circles[index]);
            return 1;
        }

        int append(Circle circle){
            if (isFull()) return 0;
            circles[n]=new Circle;
            *circles[n]=circle;
            n++;
            return 1;
        }
        /*
        int searchCircle(Circle* circle){
            for(int i=0;i<n;i++){
                if (circles[n]==circle) return i;
            }
            return -1;
        }*/
        int deleteCS(int index){
            if (isEmpty() || index>=n) return 0;
            /*int index=searchCircle(circle);
            if (index==-1) return 0;*/
            delete circles[index];
            for (int i=index;i<n-1;i++){
                circles[i]=circles[i+1];
                circles[i]->title.setString(convertItoS(i));
            }
            n--;
            return 1;
        }

        void deleteAll(){
            if (isEmpty()) return;
            int rounds=n;
            for (int i=0;i<rounds;i++){
                deleteCS(0);
            }
            circles[0]=NULL;
        }
} ;


class ArrowLine: public Drawable, public Transformable{
    private:
        VertexArray line;
        VertexArray arrow;
        VertexArray curvedLine;
        Text imNum; // trong so
        void loadLine(Vector2f head, Vector2f tail,Color color){
            line[0].position=head;
            line[1].position=tail;

            line[0].color=color;
            line[1].color=color;
        }
        void loadArrow(Vector2f head, Vector2f tail,int deltaX,int deltaY,int offset,Color color){
            int offsetX=(tail.x-head.x);
            int offsetY=(tail.y-head.y);
            Vector2f middlePoint((head.x+tail.x)/2,(head.y+tail.y)/2);
            float x1,x2,a,b;
            pathFunction(head.x,head.y,tail.x,tail.y,&a,&b);
            distancePt(middlePoint.x,middlePoint.y,offset,a,b,&x1,&x2);

            //if (x1<x2) swapNum(&x1,&x2); // x1>=x2
            float pivotPtX=(offsetX>0)?x2:x1;
            float pivotPtY=tinhY(pivotPtX,a,b);
            Vector2f pivotPoint(pivotPtX,pivotPtY);
            Vector2f point1;
            Vector2f point2;
            if ((offsetX>0 && offsetY<0) || (offsetX<0 && offsetY>0)){
                point1=Vector2f(pivotPoint.x-deltaX,pivotPoint.y-deltaY);
                point2=Vector2f(pivotPoint.x+deltaX,pivotPoint.y+deltaY);
            }
            else{
                point1=Vector2f(pivotPoint.x-deltaX,pivotPoint.y+deltaY);
                point2=Vector2f(pivotPoint.x+deltaX,pivotPoint.y-deltaY);
            }

            arrow[0].position=point1;
            arrow[1].position=middlePoint;
            arrow[2].position=point2;

            arrow[0].color=color;
            arrow[1].color=color;
            arrow[2].color=color;

        }
        Vector2f calculateBezierPt(float t,Vector2f p0,Vector2f p1,Vector2f p2,Vector2f p3){
            float u=1-t;
            float x=u*u*u*p0.x+3*u*u*t*p1.x+3*u*t*t*p2.x+t*t*t*p3.x;
            float y=u*u*u*p0.y+3*u*u*t*p1.y+3*u*t*t*p2.y+t*t*t*p3.y;
            return Vector2f(x,y);
        }
        void loadCurvedLine(Vector2f head,Vector2f tail,Color color){
            Vector2f p0=head;
            Vector2f p1,p2;
            // is tail_x greater than head_x ?
            bool tgh_x=(tail.x>=head.x)?true:false;
            bool tgh_y=(tail.y>=head.y)?true:false;
            // tinh duong thang vuong goc tai head, tail
            float a1,b1,a2,b2,a3,b3;
            pathFunction(head.x,head.y,tail.x,tail.y,&a1,&b1);
            perpendicularFunction(head.x,head.y,a1,b1,&a2,&b2);
            perpendicularFunction(tail.x,tail.y,a1,b1,&a3,&b3);

            float x2_1,x2_2,x3_1,x3_2;
            // x2_1>=x2_2 and x3_1>=x3_2
            distancePt(head.x,head.y,DISTANCEBEZIERCURVE,a2,b2,&x2_1,&x2_2);
            distancePt(tail.x,tail.y,DISTANCEBEZIERCURVE,a3,b3,&x3_1,&x3_2);

            if ((tgh_x && tgh_y) || (tgh_x && !tgh_y)){
                p1=Vector2f(x2_1,tinhY(x2_1,a2,b2));
                p2=Vector2f(x3_1,tinhY(x3_1,a3,b3));
            }
            else if ((!tgh_x && tgh_y) || (!tgh_x && !tgh_y)) {
                p1=Vector2f(x2_2,tinhY(x2_2,a2,b2));
                p2=Vector2f(x3_2,tinhY(x3_2,a3,b3));
            }

            Vector2f p3=tail;
            Vector2f subHead,subTail;

            for (int i=0;i<CURVEDSEGMENT;i++){
                float t=i/(float)CURVEDSEGMENT;
                curvedLine[i].position=calculateBezierPt(t,p0,p1,p2,p3);
                curvedLine[i].color=color;
                if (i==11*CURVEDSEGMENT/12) subHead=calculateBezierPt(t,p0,p1,p2,p3);
                else if (i==11*CURVEDSEGMENT/12+1) subTail=calculateBezierPt(t,p0,p1,p2,p3);
            }
            loadArrow(subHead,subTail,5,5,5,color);
        }
        virtual void draw(RenderTarget& target, RenderStates states) const{
            states.transform*=getTransform(); // apply transform
            target.draw(line);
            target.draw(arrow);
            target.draw(curvedLine);
        }
    public:
        ArrowLine(){
            line.setPrimitiveType(LineStrip);
            line.resize(2);
            arrow.setPrimitiveType(LineStrip);
            arrow.resize(3);
            curvedLine.setPrimitiveType(LineStrip);
            curvedLine.resize(CURVEDSEGMENT);
        }
        void loadArrowLine(Vector2f head, Vector2f tail,Color color){
            loadLine(head,tail,color);
            loadArrow(head,tail,5,5,10,color);
        }
        void loadArrowCurvedLine(Vector2f head, Vector2f tail, Color color){
            loadCurvedLine(head,tail,color);
        }
};

class ArrowLineList{
    private:

    public:
        ArrowLine *lines[MAXNODE][MAXNODE];
        //ArrowLine lines[MAXNODE][MAXNODE];
        int n;
        ArrowLineList(){
            this->n=0;
            /*lines=new ArrowLine*[MAXNODE];
            for (int i=0;i<MAXNODE;i++){
                lines[i]=new ArrowLine[MAXNODE];
            }*/
        }
        bool isFull(){
            return n==MAXNODE*MAXNODE;
        }
        bool isEmpty(){
            return n==0;
        }
        int append(ArrowLine path,int head,int tail){
            if (isFull()) return 0;

            lines[head][tail]=new ArrowLine;
            *(lines[head][tail])=path;
            n++;
            return 1;
        }

        int deleteAL(int head,int tail){
            if (isEmpty() || lines[head][tail]==NULL) return 0;
            delete lines[head][tail];
            lines[head][tail]=NULL;
            n--;
            return 1;
        }

        void deleteAll(){
            if (isEmpty()) return;
            for (int i=0;i<MAXNODE;i++){
                for (int k=0;k<MAXNODE;k++){
                    deleteAL(i,k);
                }
            }
        }

};

typedef int Graph[MAXNODE][MAXNODE];

// danh sach chua lua chon node de ve duong di
class NodeList{
    private:
        int arr[2];
        int n;
    public:
        NodeList(){
            n=0;
            for (int i=0;i<2;i++) arr[i]=-1;
        }
        bool isFull(){return n==2;}
        bool isEmpty(){return n==0;}
        int append(int x){
            if (isFull()) return 0;
            arr[n++]=x;
            return 1;
        }
        int getHead(){
            if (isEmpty()) return -1;
            if (arr[0]!=-1) return arr[0];
            return -1;
        }
        int getTail(){
            if (isEmpty()) return -1;
            if (arr[1]!=-1) return arr[1];
            return -1;
        }
        void reset(){
            n=0;
            for (int i=0;i<2;i++) arr[i]=-1;
        }
        int getN(){return n;}
};



void deleteLastChar(char* str,int n){
    for (int i=0;i<n;i++){
        if (str[i]=='\0') {str[i-1]='\0';return;}
    }
}

void copyTitle(Text* title,Text* temp){
    title->setCharacterSize(temp->getCharacterSize());
    title->setPosition(temp->getPosition());
    title->setFillColor(temp->getFillColor());
    title->setFont(myFont);
    title->setString(temp->getString());
}

class Diagram: public Drawable, public Transformable{
    private:
        virtual void draw(RenderTarget& target, RenderStates states) const{
            states.transform*=getTransform(); // apply transform
            int n=nodes.n;
            for(int k=0;k<n;k++){
                for(int j=0;j<n;j++){
                    ArrowLine *temp=paths.lines[k][j];
                    if (temp!=NULL) target.draw(*temp);
                }
            }
            for(int i=0;i<nodes.n;i++){
                target.draw(*(nodes.circles[i]));
            }

        }

        void resetMatrix(){
            for (int i=0;i<MAXNODE;i++){
                for (int k=0;k<MAXNODE;k++){
                    matrix[i][k]=0;
                }
            }
        }
    public:
        ArrowLineList paths;
        Graph matrix;
        CirclesList nodes;
        Diagram(){
            //matrix= new int*[MAXNODE];
            resetMatrix();
        }
        void addNode(Vector2i centerPoint){
            Circle node;
            int x=nodes.n;
            string index=convertItoS(x);
            Vector2f startPt=Vector2f(centerPoint.x,centerPoint.y);
            node.loadCircle(startPt,RADIUS,index);
            nodes.append(node);
        }
        void deleteNode(int index){
            int n=nodes.n;
            nodes.deleteCS(index);
            for (int i=0;i<n;i++){
                deletePath(index,i);
                if (matrix[i][index]!=0){
                    deletePath(i,index);
                }
            }
            if (index<n-1){
                for (int i=0;i<nodes.n;i++){
                    matrix[index][i]=matrix[index+1][i];
                    matrix[index+1][i]=0;
                    matrix[i][index]=matrix[i][index+1];
                    matrix[i][index+1]=0;
                    paths.lines[index][i]=paths.lines[index+1][i];
                    paths.lines[index+1][i]=NULL;
                    paths.lines[i][index]=paths.lines[i][index+1];
                    paths.lines[i][index+1]=NULL;
                }

            }

        }
        void addPath(int headNode,int tailNode,int num,Color color){
            ArrowLine path;
            Circle *startNode=nodes.circles[headNode];
            Circle *endNode=nodes.circles[tailNode];
            if (matrix[headNode][tailNode]!=0) {
                matrix[headNode][tailNode]=num;
                return;
            }
            if (matrix[tailNode][headNode]==0) path.loadArrowLine(startNode->getCenter(),endNode->getCenter(),color);
            else path.loadArrowCurvedLine(startNode->getCenter(),endNode->getCenter(),color);
            paths.append(path,headNode,tailNode);
            matrix[headNode][tailNode]=num;
        }
        void deletePath(int headNode,int tailNode){
            paths.deleteAL(headNode,tailNode);
            matrix[headNode][tailNode]=0;
        }
        CirclesList getNodes(){return nodes;}
        int getNumOfNodes(){return nodes.n;}

        void reset(){
            paths.deleteAll();
            nodes.deleteAll();
            resetMatrix();
        }
};

class MatrixText:public Drawable, public Transformable{
    private:
        int n;
        Text *matrix[MAXNODE+1]; // +1 for the title
        InfoBox container;

        Text loadText(string text,Vector2f start,Font font=myFont,
             Color textColor=Color::Black,int charSize=CHARSIZEMATRIXTEXT){
            Text result;
            result.setString(text);
            result.setFont(font);
            result.setPosition(start);
            result.setFillColor(textColor);
            result.setCharacterSize(charSize);
            return result;
        }
        int append(Text text){
            if (isFull()) return 0;
            matrix[n]=new Text;
            *(matrix[n])=text;
            n++;
            return 1;
        }
        virtual void draw(RenderTarget& target, RenderStates states) const{
            states.transform*=getTransform(); // apply transform
            for (int i=0;i<this->n;i++){
                Text *temp=matrix[i];
                Text a;
                copyTitle(&a,temp);
                target.draw(a);
            }
        }
    public:
        MatrixText(){
            matrix[0]=new Text;
            n=1;
        }

        InfoBox getContainer(){
            return container;
        }

        bool isEmpty(){return n==0;}
        bool isFull(){return n==MAXNODE+1;}

        void load(Font font,char* intro,Vector2f startBox,Vector2f sizeBox, Color boxColor){
            container.load(font,intro,startBox,sizeBox,boxColor);
            Vector2f titleStart=Vector2f(startBox.x+CHARSIZEMATRIXTEXT+10,
                 container.getTitle().getPosition().y+CHARSIZEMATRIXTEXT+10+container.getTitle().getCharacterSize());
            loadTitle(titleStart,font);
        }

        // delete row and column
        int deleteRow(int row){
            if (isEmpty() || row>=n) return 0;
            delete matrix[row];
            for (int i=row;i<n-1;i++){
                matrix[i]=matrix[i+1];
            }
            n--;
            return 1;
        }

        void deleteAll(){
            if (isEmpty()) return;
            int rounds=n-1;
            for (int i=0;i<rounds;i++){
                deleteRow(1);
            }
            matrix[0]->setString("");
        }

        int appendRow(Diagram *graph,Font font=myFont,
                      Color textColor=Color::Black,int charSize=CHARSIZEMATRIXTEXT){
            if (isFull()) return 0;
            int row=graph->nodes.n-1;
            string text=convertItoS(row)+" ";
            for (int i=0;i<graph->nodes.n;i++){
                text+=convertItoS(graph->matrix[row][i])+" ";
            }
            Text title=*(matrix[0]);

            Vector2f start=Vector2f(title.getPosition().x-CHARSIZEMATRIXTEXT,
                                    title.getPosition().y+(CHARSIZEMATRIXTEXT+10)*n);
            append(loadText(text,start,font,textColor,charSize));
            return 1;
        }

        void loadTitle(Vector2f start,Font font=myFont,
             Color textColor=Color::Black,int charSize=CHARSIZEMATRIXTEXT){
            string title=" ";
            for (int i=0;i<n-1;i++){
                title+=convertItoS(i)+" ";
            }
            *(matrix[0])=loadText(title,start,font,textColor,charSize);
        }

        void updateTitle(){
            string title=" ";
            for (int i=0;i<n-1;i++){
                title+=convertItoS(i)+" ";
            }

            matrix[0]->setString(title);
        }

        // update string after adding nodes, adding paths
        void loadRow(Diagram *graph,int row){
            string newText=convertItoS(row)+" ";
            int x=matrix[row+1]->getPosition().x;
            for (int i=0;i<graph->nodes.n;i++){
                newText+=convertItoS(graph->matrix[row][i])+" ";

            }
            matrix[row+1]->setString(newText);
            matrix[row+1]->setPosition(x,matrix[0]->getPosition().y+(CHARSIZEMATRIXTEXT+10)*(row+1));
        }

        void loadAllRow(Diagram *graph){
            for (int k=0;k<n-1;k++){
                loadRow(graph,k);
            }
        }
};


class TextInputBox{
    private:
        InfoBox mainBox;
        Text input;
        RectangleShape inputBox;
        InfoBox okeOption;
        InfoBox cancleOption;

        virtual void draw(RenderTarget& target, RenderStates states) const{
            //states.transform*=getTransform(); // apply transform
            target.draw(inputBox);
            //target.draw(title);
        }

        void loadInput(char* content, Vector2f start, Font font,Color textColor, int charSize){
            input.setString(content);
            input.setPosition(start);
            input.setFont(font);
            input.setFillColor(textColor);
            input.setCharacterSize(charSize);
        }

        void loadInputBox(Vector2f start, Vector2f boxSize, Color color, Color outlineColor=Color::Black, int thickness=2.5){
            inputBox.setPosition(start);
            inputBox.setFillColor(color);
            inputBox.setSize(boxSize);
            inputBox.setOutlineColor(outlineColor);
            inputBox.setOutlineThickness(thickness);
        }
    public:


        void load(char* title,Font font,Vector2f start,Vector2f boxSize,Color boxColor,
                   Vector2f inputBoxSize,Color inputBoxColor,Vector2f optionBoxSize,
                   Color optionBoxColor,Color textColor=Color::Black,int inputCharSize=CHARSIZEINPUTBOX){
            mainBox.load(font,title,start,boxSize,boxColor);
            Vector2f inputBoxStart(start.x+boxSize.x/2-inputBoxSize.x/2, start.y+boxSize.y/2-inputBoxSize.y/2);
            loadInputBox(inputBoxStart,inputBoxSize,inputBoxColor);
            Vector2f okeOptionStart(inputBoxStart.x+inputBoxSize.x/4-optionBoxSize.x/2,
                                    start.y+boxSize.y*3/4+boxSize.y/8-optionBoxSize.y/2);
            Vector2f cancleOptionStart(okeOptionStart.x+inputBoxSize.x/2,okeOptionStart.y);
            okeOption.load(font,"OK",okeOptionStart,optionBoxSize,optionBoxColor);
            cancleOption.load(font,"CANCLE",cancleOptionStart,optionBoxSize,optionBoxColor);
            Vector2f textStart(inputBoxStart.x+5,inputBoxStart.y+inputCharSize);
            loadInput("",textStart,font,textColor,inputCharSize);
        }

        void loadErrorBox(char* title,Font font,Vector2f start,Vector2f boxSize,Color boxColor,
                   Vector2f optionBoxSize,Color optionBoxColor,Color textColor=Color::Black,
                   int inputCharSize=CHARSIZEINPUTBOX){
            mainBox.load(font,title,start,boxSize,boxColor);

            Vector2f okeOptionStart(start.x+boxSize.x/4-optionBoxSize.x/2,
                                    start.y+boxSize.y*3/4-optionBoxSize.y/2);
            Vector2f cancleOptionStart(okeOptionStart.x+boxSize.x/2,okeOptionStart.y);
            okeOption.load(font,"OK",okeOptionStart,optionBoxSize,optionBoxColor);
            cancleOption.load(font,"CANCLE",cancleOptionStart,optionBoxSize,optionBoxColor);
       }

       void loadNoticeBox(char* title,Font font,Vector2f start,Vector2f boxSize,Color boxColor,
                   Vector2f optionBoxSize,Color optionBoxColor,Color textColor=Color::Black,
                   int inputCharSize=CHARSIZEINPUTBOX){
            mainBox.load(font,title,start,boxSize,boxColor);

            Vector2f okeOptionStart(start.x+boxSize.x/2-optionBoxSize.x/2,
                                    start.y+boxSize.y*3/4-optionBoxSize.y/2);
            okeOption.load(font,"OK",okeOptionStart,optionBoxSize,optionBoxColor);
       }

        void setTextInput(string text){
            this->input.setString(text);
        }

        void setTitle(string title){
            mainBox.reloadTitle(title);
        }



        InfoBox getMainbox(){
            return mainBox;
        }

        InfoBox getOkeOption(){
            return okeOption;
        }

        InfoBox getCancleOption(){
            return cancleOption;
        }

        RectangleShape getInputBox(){
            return inputBox;
        }

        Text getText(){
            return input;
        }

        void resetText(){
            input.setString("");
        }
};


void drawMatrixText(RenderWindow* window,MatrixText* matrix){

    InfoBox box=matrix->getContainer();
    drawInfoBox(window,&box);
    window->draw(*matrix);
}

void initializeWindow(RenderWindow* window){
    window->create(VideoMode(WINDOWWIDTH,WINDOWHEIGHT),"Hello");
    window->setFramerateLimit(60);
    window->setPosition(Vector2i(SCREENWIDTH/2-WINDOWWIDTH/2,SCREENHEIGHT/2-WINDOWHEIGHT/2));
}


void drawIconSet(RenderWindow* window,IconSet* myIconSet){
    window->draw(*myIconSet);
}



void drawInfoBox(RenderWindow* window,InfoBox* myBox){
    //cout << "dddd" << endl;
    Text x=myBox->getTitle();
    Text y;
    copyTitle(&y,&x);
    x=myBox->getContent();
    Text z;
    copyTitle(&z,&x);
    window->draw(*myBox);
    window->draw(y);
    window->draw(z);

}

void drawTextInputBox(RenderWindow* window,TextInputBox* textInputBox){
    InfoBox main=textInputBox->getMainbox();
    InfoBox oke=textInputBox->getOkeOption();
    InfoBox cancle=textInputBox->getCancleOption();
    drawInfoBox(window,&main);
    drawInfoBox(window,&oke);
    drawInfoBox(window,&cancle);
    Text x = textInputBox->getText();
    Text y;
    copyTitle(&y,&x);
    window->draw(textInputBox->getInputBox());
    window->draw(y);
}

bool isInFrame (Vector2i cursorPos,Vertex* quad){
    int x=cursorPos.x;
    int y=cursorPos.y;
    if (x>=quad[0].position.x && x<=quad[1].position.x
        && y>=quad[0].position.y && y<=quad[2].position.y){return true;}
    return false;
}

bool isInBox(RectangleShape box,Vector2i mousePos){
    int x=mousePos.x;
    int y=mousePos.y;
    if (x<=(box.getPosition().x+box.getSize().x) && x>=box.getPosition().x &&
        y<=(box.getPosition().y+box.getSize().y) && y>=box.getPosition().y) return true;
    return false;
}

void effectHover(Vertex* quad,VertexArray* boxQuad){
    (*boxQuad)[0].position=Vector2f(quad[0].position.x-UNITPIXEL,quad[0].position.y-UNITPIXEL);
    (*boxQuad)[1].position=Vector2f(quad[1].position.x+UNITPIXEL,quad[1].position.y-UNITPIXEL);
    (*boxQuad)[2].position=Vector2f(quad[2].position.x+UNITPIXEL,quad[2].position.y+UNITPIXEL);
    (*boxQuad)[3].position=Vector2f(quad[3].position.x-UNITPIXEL,quad[3].position.y+UNITPIXEL);

    (*boxQuad)[0].texCoords=Vector2f(0,0);
    (*boxQuad)[1].texCoords=Vector2f(BOXSIZE.x,0);
    (*boxQuad)[2].texCoords=Vector2f(BOXSIZE.x,BOXSIZE.y);
    (*boxQuad)[3].texCoords=Vector2f(0,BOXSIZE.y);

}

int hoveredIcon(int numOfIcon,IconSet* iconSet,VertexArray* boxQuad,Vector2i mousePos){
    VertexArray vertices=iconSet->getVertices();
    for(int i=0;i<numOfIcon;i++){
        Vertex* quad=&vertices[i*4];
        if (isInFrame(mousePos,quad)){
            return i;
        }
    }
    return -1;
}

void drawSelectIcon(int x,IconSet* iconSet,VertexArray* boxQuad,bool* flag){
    *flag=false;
    VertexArray vertices=iconSet->getVertices();
    if(x!=-1){
        *flag=true;
        Vertex* quad=&vertices[x*4];
        effectHover(quad,boxQuad);
    }
}

void drawSelectIcon(int x,IconSet* iconSet,VertexArray* boxQuad){
    VertexArray vertices=iconSet->getVertices();
    if(x!=-1){
        Vertex* quad=&vertices[x*4];
        effectHover(quad,boxQuad);
    }
}

void initilizeDemoBoxes( InfoBox* infoBoxGroup,Vector2f start,Vector2f buttonSize,Font font,unsigned int charSize,
                        Color buttonColor,unsigned int gap, Color lineColor=Color::Black, Color textColor=Color::Black,
                        unsigned int thickness=2.5, Color shadeColor=Color::Black ){
    int z=start.y;
    for (int i=1;i<=NUMOFDEMOS;i++){
        int x,y;
        if (i%2!=0) x=start.x;
        else x=start.x+buttonSize.x+gap;
        if (i>1 && i%2!=0) z+=buttonSize.y+SHADESIZE.y+gap;
        y=z;
        Vector2f buttonStart(x,y);
        infoBoxGroup[i-1].load(font,DEMO[i-1],buttonStart,buttonSize,buttonColor,charSize,
                             shadeColor,lineColor,textColor,thickness);
    }
}

void drawDemoBoxes(RenderWindow* window, InfoBox* infoBoxGroup){
    for(int i=0;i<NUMOFDEMOS;i++){
        drawInfoBox(window,&infoBoxGroup[i]);
    }
}

int getSelectedDemoButt(InfoBox* infoBoxGroup, Vector2i mousePos){
    for(int i=0;i<NUMOFDEMOS;i++){
        if (isInBox(infoBoxGroup[i].getContainBox(),mousePos)){
            return i;
        }
    }
    return -1;
}

void loadSelectedDemoButt(InfoBox* button){
    Vector2f temp=button->getShadeBox().getSize();
    button->setColorContainBox(SELECTBOXCOLOR);
    button->setShadeBoxPos(button->getContainBox().getPosition());
    button->moveShadeBox(REVERSEDSHADESIZE);
    button->setShadeBoxSize(Vector2f(temp.x+SHADESIZE.x+2,temp.y+SHADESIZE.y+2));
}

void loadUnselectedDemoButt(InfoBox* button){
    button->setColorContainBox(BACKGROUNDCOLOR);
    button->setShadeBoxPos(button->getContainBox().getPosition());
    button->setShadeBoxSize(button->getContainBox().getSize());
    button->moveShadeBox(SHADESIZE);
}

int okeOrCancle (TextInputBox *textInputBox, Vector2i mousePos){
    if (isInBox(textInputBox->getOkeOption().getContainBox(),mousePos)) {
        return 0;
    }
    else if (isInBox(textInputBox->getCancleOption().getContainBox(),mousePos)) {
        return 1;
    }
    return -1;
}

void addNodeFunction(Diagram* graph,Vector2i mousePos){
    graph->addNode(mousePos);
}

bool isInNode(Circle *node,Vector2i mousePos){
    int x=node->shape.getPosition().x;
    int y=node->shape.getPosition().y;
    float radius=node->shape.getRadius();
    if (mousePos.x>=x && mousePos.x<=x+2*radius &&
        mousePos.y>=y && mousePos.y<=y+2*radius) return true;
    return false;
}

// invalid pos when add a circle which must be out of any info boxes
bool isInvalidPos(Vector2i mousePos,RectangleShape *recList,int n){
    int x=mousePos.x;
    int y=mousePos.y;
    Vector2i p1(x,y+RADIUS);
    Vector2i p2(x+RADIUS,y);
    Vector2i p3(x+2*RADIUS,y+RADIUS);
    Vector2i p4(x+RADIUS,y+2*RADIUS);
    for (int i=0;i<n;i++){
        RectangleShape temp=recList[i];
        if (isInBox(temp,p1) || isInBox(temp,p2) || isInBox(temp,p3) || isInBox(temp,p4)) return true;
    }
    return false;
}

int getSelectedNode(Diagram* graph,Vector2i mousePos){
    for(int i=0;i<graph->nodes.n;i++){
        if (isInNode((graph->nodes.circles[i]),mousePos)) return i;
    }
    return -1;
}

void loadSelectedNode(Diagram* graph,int index){
    graph->nodes.circles[index]->shape.setFillColor(SELECTBOXCOLOR);
}

void loadUnselectedNode(Diagram* graph,int index){
    graph->nodes.circles[index]->shape.setFillColor(NODECOLOR);
}

void addPathFunction(Diagram* graph,NodeList *selectedNodes,Vector2i mousePos,MatrixText* matrixNode,Color color){
    int seNode=getSelectedNode(graph,mousePos);
    if (seNode!=-1){
        if (!selectedNodes->isEmpty() && seNode==selectedNodes->getHead()) return;
        selectedNodes->append(seNode);
        loadSelectedNode(graph,seNode);
        if (selectedNodes->isFull()){
            graph->addPath(selectedNodes->getHead(),selectedNodes->getTail(),4,color); // cho dai trong so
            loadUnselectedNode(graph,selectedNodes->getHead());
            loadUnselectedNode(graph,selectedNodes->getTail());
            matrixNode->loadRow(graph,selectedNodes->getHead());
            selectedNodes->reset();
        }
    }
}

char* stringToCharArray(string x){
    char *result = new char[x.length()+1];
    strcpy(result,x.c_str());
    return result;
}

bool containsInvalidChar(string text){
    for (int i=0;i<text.size();i++){
        if (text[i]==' ' || text[i]=='\n' || text[i]=='\t') return true;
    }
}

bool isFileExist(string filePath){
    FILE *f=fopen(stringToCharArray(filePath),"r");
    fclose(f);
    return f!=NULL;
}

 // save graph file
int saveGraphFile(string filePath,Diagram *graph){
    FILE *f=fopen(stringToCharArray(filePath),"w");
    int n=graph->nodes.n;
    string numOfNode=convertItoS(n)+"\n"; // ghi tong so node
    fputs(stringToCharArray(numOfNode),f);
    for (int i=0;i<n;i++){
        // ghi toa do x,y cua node
        Circle temp;
        if (!graph->nodes.getCircle(i,&temp)){
            // hien thong bao loi
            cout << "Khong ton tai node nay" << endl;
            return 0;
        }
        else{
            float x= temp.shape.getPosition().x;
            float y= temp.shape.getPosition().y;
            cout << "x: " << x << "\ny: " << y << endl;
            string txt= convertItoS(x)+" "+convertItoS(y)+"\n";
            fputs(stringToCharArray(txt),f);
        }
    }
    for (int i=0;i<n;i++){
        for (int k=0;k<n;k++){
            int trongSo=graph->matrix[i][k];
            string txt=convertItoS(trongSo)+" ";
            fputs(stringToCharArray(txt),f);
        }
        fputs("\n",f);
    }
    fclose(f);
    return 1;
}

// load graph file
int loadGraphFile(string filePath,Diagram *graph,MatrixText* mt){
    FILE *f=fopen(stringToCharArray(filePath),"r");
    if (f==NULL) return 0;
    graph->reset();
    mt->deleteAll();
    char numOfNodeTXT[3];
    fscanf(f,"%s",numOfNodeTXT);
    int numOfNode=atoi(numOfNodeTXT);
    // load nodes
    for (int i=0;i<numOfNode;i++){
        char xTXT[5],yTXT[5];
        fscanf(f,"%s",xTXT);
        fscanf(f,"%s",yTXT);
        int x=atoi(xTXT);
        int y=atoi(yTXT);
        graph->addNode(Vector2i(x,y));
    }
    // load paths and matrix
    for (int i=0;i<numOfNode;i++){
        for (int k=0;k<numOfNode;k++){
            char numTXT[5];
            fscanf(f,"%s",numTXT);
            int num=atoi(numTXT);
            if (num!=0) graph->addPath(i,k,num,Color::Black);
        }
        mt->appendRow(graph);
    }
    mt->updateTitle();
    mt->loadAllRow(graph);
    fclose(f);
    return 1;
}

void deleteNode(Diagram* graph,MatrixText* mt,int indexNode){
    graph->deleteNode(indexNode);
    mt->deleteRow(indexNode+1);
    mt->updateTitle();
    mt->loadAllRow(graph);
}

void drawGraph(RenderWindow* window, Diagram* graph){
    //cout << "HEHEH" << endl;
    window->draw(*graph);
    CirclesList temp=graph->nodes;
    for(int i=0;i<temp.n;i++){
        Circle x=*(temp.circles[i]);
        Text xT=x.getTitle();
        Text yT;
        copyTitle(&yT,&xT);
        window->draw(yT);
    }

}

bool inRange(int x, int a, int b){
    return (x>=a && x<=b);
}

int** copyMatrix(Diagram *graph){
    int n=graph->nodes.n;
    int **temp=new int*[n];
    for (int i=0;i<n;i++){
        temp[i]=new int[n];
        for (int k=0;k<n;k++){
            temp[i][k]=graph->matrix[i][k];
        }
    }
    return temp;
}


int main()
{
    // --------------Declares components of Windows
    RenderWindow window;

    // Components of Icon Box
    IconSet mySet;
    VertexArray boxQuad(Quads,4); // outline when select icon
    if (!mySet.load(".\\imgs\\icons\\iconset_1.png",ICONSIZE,MYICONSET,1,NUMOFFUNCTIONICON,
                     ICONSETSTART,GAPBTICONS,BOXCOLOR)){
        return EXIT_FAILURE;
    }

    // select box
    Texture selectedBox; // texture of above outline
    if (!selectedBox.loadFromFile(".\\imgs\\icons\\select.png")) return EXIT_FAILURE;


    // font and text
    InfoBox matrixBox;
    if (!myFont.loadFromFile(".\\fonts\\8bit\\8bit.ttf")){
        return EXIT_FAILURE;
    }


    // demo buttons
    InfoBox demos[MAXDEMO]; // info box group of demo butts
    InfoBox demoFrame; // A frame contains demo buttons
    initilizeDemoBoxes(demos,DEMOBUTTONSTART,DEMOBUTTONSIZE,myFont,CHARSIZEDEMOBUTTON,BACKGROUNDCOLOR,DEMOBUTTONGAP);
    demoFrame.load(myFont,"DEMO",DEMOBOXSTART,DEMOBOXSIZE,BOXCOLOR);

    // result box
    InfoBox resultBox;
    resultBox.load(myFont,"RESULT",RESULTBOXSTART,RESULTBOXSIZE,BOXCOLOR);

    // input-option box
    TextInputBox inputBox;
    inputBox.load("Hello",myFont,TEXTINPUTFRAMESTART,TEXTINPUTFRAMESIZE,BOXCOLOR,TEXTINPUTBOXSIZE,
                  SELECTBOXCOLOR,OPTIONBOXSIZE,BACKGROUNDCOLOR);

    // input-file box
    //TextInputBox fileInputBox;
    //fileInputBox.load("NHAP TEN FILE",myFont,TEXTINPUTFRAMESTART,TEXTINPUTFRAMESIZE,BOXCOLOR,TEXTINPUTBOXSIZE,
    //              SELECTBOXCOLOR,OPTIONBOXSIZE,BACKGROUNDCOLOR);

    // error box
    TextInputBox errorBox;
    errorBox.loadErrorBox("Loi!!!",myFont,ERRORBOXSTART,ERRORBOXSIZE,BOXCOLOR,OPTIONBOXSIZE,BACKGROUNDCOLOR);

    // notice box
    TextInputBox noticeBox;
    noticeBox.loadNoticeBox("Thong bai",myFont,ERRORBOXSTART,ERRORBOXSIZE,BOXCOLOR,OPTIONBOXSIZE,BACKGROUNDCOLOR);

    initializeWindow(&window);
    bool flag; // flag select function
    int funcChoice=-1;
    int demoChoice=-1;
    bool showInputBox=false; // flag when to show inputBox
    bool showErrorBox=false;
    bool showNoticeBox=false;
    string fileType=TXT;
    //bool showFileInputBox=false; // flag when to show fileInputBox
    string resultInput;
    NodeList selectedNodes;
    int seNode=-1;

    // initialize MatrixText
    //matrixBox.load(myFont,"MA TRAN TRONG SO",MATRIXBOXSTART,MATRIXBOXSIZE,BOXCOLOR);
    MatrixText matrixNode;
    /*const Vector2f matrixTextTitleStart(MATRIXBOXSTART.x+CHARSIZEMATRIXTEXT+10,
            matrixBox.getTitle().getPosition().y+CHARSIZEMATRIXTEXT+10+matrixBox.getTitle().getCharacterSize());
    Vector2f matrixTextStart(matrixTextTitleStart.x-CHARSIZEMATRIXTEXT,matrixTextTitleStart.y+CHARSIZEMATRIXTEXT+3);
    matrixNode.loadTitle(matrixTextTitleStart);*/
    matrixNode.load(myFont,"MA TRAN TRONG SO",MATRIXBOXSTART,MATRIXBOXSIZE,BOXCOLOR);

    RectangleShape recList[NUMOFINFOBOXES]={matrixNode.getContainer().getContainBox(),demoFrame.getContainBox(),
                                            resultBox.getContainBox(),mySet.getBox()};

    Diagram currentGraph;
    Diagram x;

    while(window.isOpen()){
        Event event;

        if (window.pollEvent(event)){
            if(event.type==Event::Closed) {
                window.close();
            }

            if (event.type==Event::KeyPressed){
                if (event.key.code==Keyboard::Escape) window.close();
            }

            Vector2i mousePos=Mouse::getPosition(window); // get local position
            if (event.type==Event::MouseMoved){

                // ----------------- hover function icon
                if (isInBox(mySet.getBox(),mousePos)){ // in icons box
                    if (funcChoice==-1){
                        int x=hoveredIcon(NUMOFFUNCTIONICON,&mySet,&boxQuad,mousePos);
                        drawSelectIcon(x,&mySet,&boxQuad,&flag);
                    }
                }


            }

            if (event.type==Event::TextEntered){
                int unicode=event.text.unicode;
                if (showInputBox){
                    string currentText=inputBox.getText().getString();
                    if (demoChoice==-1){
                        if (MYICONSET[funcChoice]==ARROW){
                            if (inRange(unicode,49,57)){

                                if (currentText.size()==2){
                                    showNoticeBox=true;
                                    showInputBox=false;
                                    noticeBox.setTitle("Toi da la 2 chu so");
                                    continue;
                                }
                                currentText.push_back((char)unicode);
                            }

                        }
                        else{ // file functions
                            if (inRange(unicode,48,57) || inRange(unicode,65,90) || inRange(unicode,97,122)){
                                currentText.push_back((char)unicode);
                                //inputBox.setTextInput(currentText);
                            }
                        }
                    }
                    else{
                        int n=currentGraph.nodes.n;
                        if (inRange(unicode,48,48+n-1)){
                            if (currentText.size()==1) continue;
                            currentText.push_back((char)unicode);
                        }
                    }
                    if (unicode==8){ // backspace
                        if (currentText.size()==0) continue;
                        currentText.erase(currentText.length()-1);
                        //inputBox.setTextInput(currentText);

                    }
                    inputBox.setTextInput(currentText);
                }

            }

            if (event.type==Event::MouseButtonPressed){
                if(event.mouseButton.button==Mouse::Left){

                    // ------------------------ mouse press in input box
                    if (showInputBox && isInBox(inputBox.getMainbox().getContainBox(),mousePos)){
                        int selectedOption=okeOrCancle(&inputBox,mousePos);
                        if (selectedOption!=-1){
                            if (selectedOption==0){ // oke
                                resultInput=inputBox.getText().getString();
                                if (resultInput.size()==0) continue;
                                if (demoChoice!=-1){
                                    switch(MYDEMOSET[demoChoice]){
                                        case DFSf:{
                                            int n=currentGraph.nodes.n;
                                            int start=atoi(stringToCharArray(resultInput));
                                            refreshVisited();
                                            int **tempMatrix=copyMatrix(&currentGraph);
                                            int *result=DFS(tempMatrix,n,start);
                                            resultBox.reloadContent(getResultStr(result,n));
                                            deleteMatrix(tempMatrix,n);
                                            delete[] result;
                                            loadUnselectedDemoButt(demos+demoChoice);
                                            demoChoice=-1;
                                            break;
                                        }
                                        case BFSf:{
                                            int n=currentGraph.nodes.n;
                                            int start=atoi(stringToCharArray(resultInput));
                                            refreshVisited();
                                            int **tempMatrix=copyMatrix(&currentGraph);
                                            int *result=BFS(tempMatrix,n,start);
                                            resultBox.reloadContent(getResultStr(result,n));
                                            deleteMatrix(tempMatrix,n);
                                            delete[] result;
                                            loadUnselectedDemoButt(demos+demoChoice);
                                            demoChoice=-1;
                                            break;
                                        }
                                    }
                                }
                                else{
                                    switch (MYICONSET[funcChoice]){
                                        case ARROW:{
                                            if (resultInput.size()==0){
                                                showNoticeBox=true;
                                                noticeBox.setTitle("Phai nhap trong so");
                                                break;
                                            }
                                            int constant=atoi(resultInput.data());
                                            currentGraph.addPath(selectedNodes.getHead(),selectedNodes.getTail(),constant,Color::Black);

                                            loadUnselectedNode(&currentGraph,selectedNodes.getHead());
                                            loadUnselectedNode(&currentGraph,selectedNodes.getTail());
                                            matrixNode.loadRow(&currentGraph,selectedNodes.getHead());
                                            selectedNodes.reset();
                                            //inputBox.resetText();
                                            break;
                                        }
                                        case DOWNLOAD:{
                                            // kiem tra file da ton tai hay chua
                                            if (resultInput.size()==0){
                                                showNoticeBox=true;
                                                noticeBox.setTitle("Hay nhap ten file");
                                                break;
                                            }
                                            string filePath=".\\graphs\\"+resultInput+fileType;

                                            if (isFileExist(filePath)){
                                                // file da ton tai
                                                showErrorBox=true;
                                                //showInputBox=false;
                                                //cout << "vo roi!!!" << endl;
                                                errorBox.setTitle("FILE da ton tai. Ban co muon luu de file ?");
                                                break;
                                            }
                                            // file chua tao
                                            else{
                                                if (saveGraphFile(filePath,&currentGraph)) {
                                                    showNoticeBox=true;
                                                    noticeBox.setTitle("Luu thanh cong");
                                                }
                                            }
                                            //inputBox.resetText();
                                            break;
                                        }
                                        case UPLOAD:{
                                            // file khong ton tai
                                            if (resultInput.size()==0){
                                                showNoticeBox=true;
                                                noticeBox.setTitle("Hay nhap ten file");
                                                break;
                                            }
                                            string filePath=".\\graphs\\"+resultInput+fileType;
                                            if (isFileExist(filePath)){

                                                if (loadGraphFile(filePath,&currentGraph,&matrixNode)){
                                                    // hien thong bao thanh cong
                                                    cout << "Tai len thanh cong!!!" << endl;
                                                }
                                                else{
                                                    // hien thong bao that bai
                                                }
                                            }
                                            else{
                                                // hien thong bao file chua tao
                                                noticeBox.setTitle("File khong ton tai");
                                                showNoticeBox=true;
                                                //cout << "File chua tao" << endl;
                                            }
                                            //inputBox.resetText();

                                            break;
                                        }
                                    }
                                }


                            }
                            else { // cancle
                                if (demoChoice!=-1){
                                    loadUnselectedDemoButt(demos+demoChoice);
                                    demoChoice=-1;
                                }
                                else{
                                    if (MYICONSET[funcChoice]==ARROW){
                                        loadUnselectedNode(&currentGraph,selectedNodes.getHead());
                                        loadUnselectedNode(&currentGraph,selectedNodes.getTail());
                                        selectedNodes.reset();
                                    }
                                }

                            }
                            showInputBox=false;
                            inputBox.resetText();
                            //inputDone=true;
                        }
                    }

                    // -------------------------- in error box
                    else if (showErrorBox && isInBox(errorBox.getMainbox().getContainBox(),mousePos)){
                        int selectedOption=okeOrCancle(&errorBox,mousePos);
                        if (selectedOption!=-1){

                            if (selectedOption==0){ // oke
                                string filePath=".\\graphs\\"+resultInput+fileType;
                                switch (MYICONSET[funcChoice]){
                                    case DOWNLOAD:{
                                        saveGraphFile(filePath,&currentGraph);
                                        break;
                                    }
                                    case DELETE:{
                                        deleteNode(&currentGraph,&matrixNode,seNode);
                                        break;
                                    }
                                    case EDIT:{
                                        int head=selectedNodes.getHead();
                                        int tail=selectedNodes.getTail();
                                        currentGraph.deletePath(head,tail);
                                        matrixNode.loadAllRow(&currentGraph);
                                        selectedNodes.reset();
                                        break;
                                    }
                                }
                            }
                            else{ // cancle
                                int choice=MYICONSET[funcChoice];
                                if (choice==EDIT) selectedNodes.reset();
                                else if (choice!=DELETE) showInputBox=true;

                            }
                            showErrorBox=false;

                        }

                    }

                    // ----------------------- OKE in notice box
                    else if (showNoticeBox && isInBox(noticeBox.getOkeOption().getContainBox(),mousePos)){
                        showNoticeBox=false;
                        int choice=MYICONSET[funcChoice];
                        if (choice==EDIT) selectedNodes.reset();
                        else if (choice!=ADD) showInputBox=true;

                    }

                    // ----------------------- select function
                    else if (isInBox(mySet.getBox(),mousePos) && !showInputBox && !showErrorBox){ // in icons box
                        int k=hoveredIcon(NUMOFFUNCTIONICON,&mySet,&boxQuad,mousePos);
                        funcChoice=(k==-1)?funcChoice:k;
                        drawSelectIcon(funcChoice,&mySet,&boxQuad);
                        switch (MYICONSET[funcChoice]){
                            case ADD:{
                                if (!selectedNodes.isEmpty()) loadUnselectedNode(&currentGraph,selectedNodes.getHead());
                                selectedNodes.reset();
                                break;
                            }
                            case DELETE:{
                                if (!selectedNodes.isEmpty()) loadUnselectedNode(&currentGraph,selectedNodes.getHead());
                                selectedNodes.reset();
                                break;
                            }
                            case ARROW:{
                                if (!selectedNodes.isEmpty()) loadUnselectedNode(&currentGraph,selectedNodes.getHead());
                                selectedNodes.reset();
                                break;
                            }
                            case DOWNLOAD:{
                                showInputBox=true;
                                inputBox.setTitle("LUU DO THI");
                                break;
                            }
                            case UPLOAD:{
                                showInputBox=true;
                                inputBox.setTitle("TAI DO THI DA LUU");
                                break;
                            }
                        }
                    }

                    // ---------------------- select demo
                    else if(isInBox(demoFrame.getContainBox(),mousePos) && !showInputBox && !showErrorBox){ // in demo frame box
                        int selectedButt=getSelectedDemoButt(demos,mousePos); // new choice
                        if (selectedButt!=-1){
                            if (demoChoice!=selectedButt){
                                if (demoChoice!=-1) loadUnselectedDemoButt(demos+demoChoice);
                                loadSelectedDemoButt(demos+selectedButt);
                                demoChoice=selectedButt;

                                switch(MYDEMOSET[demoChoice]){
                                    case DFSf:{
                                        showInputBox=true;
                                        inputBox.setTitle("NHAP DINH BAT DAU");
                                        break;
                                    }
                                    case BFSf:{
                                        showInputBox=true;
                                        inputBox.setTitle("NHAP DINH BAT DAU");
                                        break;
                                    }
                                    case LIENTHONG:{
                                        int n=currentGraph.nodes.n;
                                        int **temp=copyMatrix(&currentGraph);
                                        ConEleMt mt=connective(temp,n);
                                        resultBox.reloadContent(getConnEleResult(mt,n));
                                        deleteMatrix(temp,n);
                                        loadUnselectedDemoButt(demos+demoChoice);
                                        demoChoice=-1;
                                        break;
                                    }
                                    case TRU:{
                                        int n=currentGraph.nodes.n;
                                        int **temp=copyMatrix(&currentGraph);
                                        int *result=duyetTru(temp,n);
                                        resultBox.reloadContent(getTruResult(result,n));
                                        deleteMatrix(temp,n);
                                        loadUnselectedDemoButt(demos+demoChoice);
                                        demoChoice=-1;
                                        break;
                                    }
                                }
                            }
                        }

                    }



                    // ------------------------ paint on canvas
                    else if(!isInBox(matrixBox.getContainBox(),mousePos) && !isInBox(resultBox.getContainBox(),mousePos)
                            && !showInputBox && !showErrorBox && !showNoticeBox){
                        switch(MYICONSET[funcChoice]){
                            case ADD:{
                                if (isInvalidPos(mousePos,recList,NUMOFINFOBOXES)){
                                    showNoticeBox=true;
                                    noticeBox.setTitle("Vi tri khong nam trong khung ve");
                                    break;
                                }
                                if (selectedNodes.getN()==1){ // reset danh sach nut da chon de ve duong di
                                    loadUnselectedNode(&currentGraph,selectedNodes.getHead());
                                    selectedNodes.reset();
                                }
                                addNodeFunction(&currentGraph,mousePos);
                                /*matrixTextStart=Vector2f(matrixTextStart.x,
                                                         matrixTextStart.y+CHARSIZEMATRIXTEXT+12);*/
                                matrixNode.appendRow(&currentGraph);
                                matrixNode.updateTitle();
                                matrixNode.loadAllRow(&currentGraph);

                                break;
                            }
                            case ARROW:{
                                // phai co du 2 nut moi ve duoc duong
                                // chua du phai thong bao
                                // neu da chon 1 node roi bam qua func khac thi phai xoa nut da chon luon

                                seNode=getSelectedNode(&currentGraph,mousePos);
                                if (seNode!=-1){

                                    if (!selectedNodes.isEmpty() && seNode==selectedNodes.getHead()) break; // chon trung node
                                    selectedNodes.append(seNode);
                                    loadSelectedNode(&currentGraph,seNode);
                                    if (selectedNodes.isFull()){
                                        inputBox.setTitle("TRONG SO ?");
                                        showInputBox=true;
                                    }
                                }
                                else {
                                    if (!selectedNodes.isEmpty()){
                                        loadUnselectedNode(&currentGraph,selectedNodes.getHead());
                                        selectedNodes.reset();
                                    }
                                }
                                break;
                            }
                            case DELETE:{
                                seNode=getSelectedNode(&currentGraph,mousePos);
                                if (seNode!=-1){
                                    showErrorBox=true;
                                    string nodeIndex=convertItoS(seNode);
                                    errorBox.setTitle("Ban co chac muon xoa nut "+nodeIndex+" ?");
                                }
                                break;
                            }
                            case EDIT:{
                                seNode=getSelectedNode(&currentGraph,mousePos);
                                if (seNode!=-1){

                                    if (!selectedNodes.isEmpty() && seNode==selectedNodes.getHead()) break; // chon trung node
                                    selectedNodes.append(seNode);
                                    loadSelectedNode(&currentGraph,seNode);
                                    if (selectedNodes.isFull()){
                                        int head=selectedNodes.getHead();
                                        int tail=selectedNodes.getTail();
                                        if (currentGraph.matrix[head][tail]==0){
                                            showNoticeBox=true;
                                            string x=convertItoS(head)+"-"+convertItoS(tail);
                                            noticeBox.setTitle("Ban chua tao duong "+x+" !");
                                        }
                                        else {
                                            showErrorBox=true;
                                            string x=convertItoS(selectedNodes.getHead())+"-"+convertItoS(selectedNodes.getTail());
                                            errorBox.setTitle("Ban co chac muon xoa duong "+x+" ?");
                                        }

                                        loadUnselectedNode(&currentGraph,head);
                                        loadUnselectedNode(&currentGraph,tail);

                                    }
                                }
                                else {
                                    if (!selectedNodes.isEmpty()){
                                        loadUnselectedNode(&currentGraph,selectedNodes.getHead());
                                        selectedNodes.reset();
                                    }
                                }
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
            }

        }
        window.clear(BACKGROUNDCOLOR);
        drawIconSet(&window,&mySet);
        if (flag) window.draw(boxQuad,&selectedBox);
        drawInfoBox(&window,&demoFrame);
        drawInfoBox(&window,&resultBox);
        drawDemoBoxes(&window,demos);
        drawGraph(&window,&currentGraph);
        drawMatrixText(&window,&matrixNode);
        if (showInputBox) drawTextInputBox(&window,&inputBox);
        if (showErrorBox) drawTextInputBox(&window,&errorBox);
        if (showNoticeBox) drawTextInputBox(&window,&noticeBox);
        window.display();
    }

    return 0;
}
