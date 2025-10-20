from maspy import *

class Agent_ag11(Agent):
    def __init__(self, agt_name):
        super().__init__(agt_name)
        self.add(Belief("chegando_no_cruzamento"))
        self.add(Goal("atravessar_com_seguranca"))

    @pl(gain, Goal("atravessar_com_seguranca"), Belief("chegando_no_cruzamento"))
    def atravessar_com_seguranca_(self, src):
        self.print("Pedindo permissao para o Controle de Trafego.")
        self.add(Belief("aguardando_sinal_verde"))

    @pl(gain, Belief("sinal_verde_recebido"), Belief("aguardando_sinal_verde"))
    def sinal_verde_recebido_(self, src):
        self.print("Sinal verde! Cruzando a via.")
        self.add(Belief("cruzamento_concluido"))

class Agent_ag12(Agent):
    def __init__(self, agt_name):
        super().__init__(agt_name)
        self.add(Belief("cruzamento_ocupado"))
        self.add(Goal("gerenciar_fluxo"))

    @pl(gain, Goal("gerenciar_fluxo"), Belief("cruzamento_ocupado"))
    def gerenciar_fluxo_(self, src):
        self.print("Verificando o status do cruzamento...")
        self.print("Cruzamento liberado.")
        self.add(Goal("enviar_sinal_verde"))

    @pl(gain, Belief("enviar_sinal_verde"))
    def enviar_sinal_verde_(self, src):
        self.print("Enviando sinal verde para o proximo veiculo.")
        self.add(Belief("sinal_verde_enviado"))


Agent_ag11("VeiculoAutonomo")
Agent_ag12("ControleDeTrafego")
Admin().start_system()
