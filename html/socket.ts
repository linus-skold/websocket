
export enum eConnectionState {
    NONE,
    OPEN,
    CLOSED,
    REFUSED,
    TIMEOUT,
}

export class Socket {
    m_State : eConnectionState 
    m_Socket : WebSocket

    constructor(url : string) {
        
        this.m_Socket = new WebSocket(url)

        this.m_Socket.onopen = function(e) {
            this.onopen(e)
        }

        this.m_Socket.onclose = function(e) {
            this.onclose(e)
        }
    }

    onopen(e : Event) : void {
        this.m_State = eConnectionState.OPEN
    }

    onclose(e : Event) : void {
        this.m_State = eConnectionState.CLOSED
    }


}


