from maspy import *

class Agent_ag8(Agent):
    def __init__(self, agt_name):
        super().__init__(agt_name)
        self.add(Belief("na_base_de_carregamento"))
        self.add(Belief("ambiente_sujo"))
        self.add(Goal("iniciar_ciclo_de_limpeza"))

    @pl(gain, Goal("iniciar_ciclo_de_limpeza"), Belief("ambiente_sujo"))
    def iniciar_ciclo_de_limpeza_(self, src):
        self.print("Saindo da base.")
        self.add(Belief("bateria_cheia"))
        self.add(Goal("procurar_sujeira"))

    @pl(gain, Belief("procurar_sujeira"), Belief("bateria_cheia"))
    def procurar_sujeira_(self, src):
        self.print("Sujeira encontrada na posicao (2,3).")
        self.add(Goal("limpar_local"))

    @pl(gain, Belief("limpar_local"))
    def limpar_local_(self, src):
        self.print("Aspirando...")
        self.print("Local limpo.")
        self.add(Goal("verificar_fim_da_limpeza"))

    @pl(gain, Belief("verificar_fim_da_limpeza"))
    def verificar_fim_da_limpeza_(self, src):
        self.print("Ambiente limpo. Retornando para a base.")
        self.add(Belief("ambiente_limpo"))


Agent_ag8("RoboAspirador")
Admin().start_system()
