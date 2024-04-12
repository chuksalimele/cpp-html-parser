#include "../include/html_parser.h"
#include "../include/element_query.h"
#include "../include/util.h"
#include <string>

using namespace chtml;


void testQuery1() {

    std::string html = R"(<abc id = `my`>
                            THIS IS ABC
                                <bbb>THIS IS BBB</bbb>
                                <def>
                                    THIS IS DEF                                    
                                    <div id='myid1'>THIS IS DIV1</div>
                                    <div id='myid2'>THIS IS DIV2</div>
                                    <div id='myid3'>
                                        THIS IS DIV3
                                        <div id='myid4' class="top div-cls">THIS IS DIV4</div>
                                        <div id='myid5' class="center div-cls">THIS IS DIV5</div>
                                        <div id='myid6' class="bottom div-cls">THIS IS DIV6</div>
                                    </div>
                                </def>
                                <div class="div-cls success">THIS IS DIV OUTER1</div>
                                <div class="div-cls fail">THIS IS DIV OUTER2</div>                                
                        </abc>)";
    

    
    size_t index = 0;
    
    HTMLNode root = parseHTML(html);    

    HTMLNode el = findElementById(root, "myid3");  
    
    std::cout << el.innerHTML << std::endl;

    std::vector<HTMLNode> els = findAllElementsByClassName(root, "div-cls");

    std::cout << "findAllElementsByClassName" << std::endl;
    for(HTMLNode n : els){
        std::cout << "----------" << std::endl;
        std::cout << n.innerHTML << std::endl;
        std::cout << "----------" << std::endl;
    }

    HTMLNode elc = findFirstElementByClassName(root, "div-cls");

    std::cout << elc.innerHTML << std::endl;

    HTMLNode ele = findElementByElementId(root, "myid3", "myid4");

    std::cout << ele.innerHTML << std::endl;

    HTMLNode elt = findFirstElementByTag(root, "def");

    std::cout << elt.innerHTML << std::endl;

    std::vector<HTMLNode> elts = findAllElementsByTag(root, "Div");
    std::cout << "findAllElementsByTag" << std::endl;
    for(HTMLNode n : elts){
        std::cout << "----------" << std::endl;
        std::cout << n.innerHTML << std::endl;
        std::cout << "----------" << std::endl;
    }
}
