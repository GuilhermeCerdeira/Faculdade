from maspy import *

class Agent_ag7(Agent):
    def __init__(self, agt_name):
        super().__init__(agt_name)
        self.add(Belief("hora do almoco"))
        self.add(Goal("preparar refeicao para 2 pessoas"))

    @pl(gain, Goal("preparar refeicao para 2 pessoas"), Belief("hora do almoco"))
    def preparar_refeicao_para_2_pessoas_(self, src):
        self.print("verificar ingredientes na geladeira")
        self.add(Goal("ligar fogao a 180 graus"))

    @pl(gain, Belief("ligar fogao a 180 graus"))
    def ligar_fogao_a_180_graus_(self, src):
        self.print("assar por 30 minutos")


Agent_ag7("robo_assistente_2000")
Admin().start_system()
