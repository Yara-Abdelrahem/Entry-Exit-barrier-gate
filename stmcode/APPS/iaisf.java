
STM -> 000001111111111111111

class server{
    boolean Entryprev = false;
    boolean Entrycurrent = garageCom.openGateState;

    boolean Exitprev = false;
    boolean Exitcurrent = garageCom.openGateState;

    if(Entrycurrent == true && Entrycurrent!= prev){
        Thread th = new thread(){
            public void run(){
                sendCommand(opengate);
                th.sleep(5000);
                sendcommad(closegate);
            }
        }
        th.start();
    }

    if(current == true && current!= prev){
        Thread th = new thread(){
            public void run(){
                sendCommand(opengate);
                th.sleep(5000);
                sendcommad(closegate);
            }
        }
        th.start();
    }


    prev = current;
}