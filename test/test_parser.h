#include "../include/html_parser.h"
#include "../include/element_query.h"
#include "../include/util.h"
#include <string>

using namespace chtml;


void testParseHTML1() {
    
    std::string html = "<a id = 'my'><input/><b> B </b><input/><c> C </c><d> D </d></a>";
    
    HTMLNode root = parseHTML(html);    

    deallocateDoc(&root);
    std::string str = std::string(root.innerHTML);
}

void testParseHTML2() {
    
    std::string html = "<a><!--1234--><input/><b> me </b><c> you </c>56789</a>";
    
    HTMLNode root = parseHTML(html);    


}
void testParseHTML3() {
    
    std::string html = "<a>abc<!--1234-->def</a>";
    
    HTMLNode root = parseHTML(html);    


}
void testParseHTML4() {
    
    std::string html = "<a id = 'myid' class =  'myclass' ></a>";
    
    HTMLNode root = parseHTML(html);    


}
void testParseHTML5() {

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
    
    
    HTMLNode root = parseHTML(html);    


}

void testParseHTML6() {
   
   std::string html = R"(<abc>
                        THIS IS A 
                            <ccc>
                                <e>
                                    <g></g>
                                    <h></h>
                                </e>
                                <f></f>
                            </ccc>
                            <d></d>
                        </abc>)";
     

    
    
    HTMLNode root = parseHTML(html);    


}

void testParseHTMLPageTag() {
   
   std::string html = R"(<html>
                            <body><p><p>INVALID - PAGE CANNOT BE DIRECT CHILD OF PAGE</p></p></body>
                        </html>)";
     

    
    
    HTMLNode root = parseHTML(html);    


}

void testParseHTMLScript() {
   
   std::string html = R"(<html>
                            <div "=""></div>
                            <div a="b"c d e f></div>
                            <div "b"c"d"e"f></div>  
                            <div "b"c"d"e"f"></div>                           
                            <div }b}c}d}e}f}></div>
                            <script href="www.something.com/url" async defer>
                                var n = 'the is variable';
                                n = 4+6;        
                            </script>
                            <body>
                            <script>
                                <div>INVALID - DIV INSIDE SCRIPT</div><a>this is anchor</a>
                            </script>
                            </body>
                        </html>)";
     

    HTMLNode root = parseHTML(html);    

}


void testParseHTMLMissingClosingTag() {
   
   /*std::string html = R"(<html>
                            <body><div><script>This is script</body>
                        </html>)";*/
     
    /*std::string html = R"(<html>
                            <body><script><div>This is script</body>
                        </html>)";*/
    
    std::string html = R"(<html>
                            <body><div><div>This is script</div></body>
                        </html>)";

    HTMLNode root = parseHTML(html);    

}