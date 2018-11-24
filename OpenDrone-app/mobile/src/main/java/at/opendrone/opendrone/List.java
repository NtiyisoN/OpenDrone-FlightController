/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package at.opendrone.opendrone;

/**
 *
 * @author 20150032
 */
public class List<T> {
    Node head, tail;
    int size;

    public List() {
        head=null;
        tail=null;
    }
    
    public void append(T val){
        Node p = new Node<T>(val);
        if(head==null){
            head = p;
        }else{
            p.prev=tail;
            tail.next=p;
        }
        tail=p;
        size++;
    }
    
    public Node<T> get(int i){
        int cnt = 0;
        Node cur = head;
        if(i==0){
            return cur;
        }
        while(cur!=null && cnt!=i){
            if(cnt==i){
                return cur;
            }cnt++;
            cur=cur.next;
        }
        return null;
    }
    
    public int size(){
        return size;
    }
    
    public void prepend(T val){
        Node p = new Node<T>(val);
        
        if(head==null){
            head=p;
        }else{
            head.prev=head;
            p.next=head;
        }
        head=p;
        size++;
    }
    
    public boolean delete(int val){
        Node cur  = head;
        Node prev = null;
        while(cur!=null&&!cur.val.equals(val)){
            prev=cur;
            cur=cur.next;
        }
        if(cur==null){
            return false;
        }else{
            if(cur==head){
                head=cur.next;
            }else if(cur==tail){
                tail=cur.prev;
                tail.next=null;
            }else{
                prev.next=cur.next;
            }
        }
        size--;
        return true;
    }
    
    public boolean remove(int i ){
        Node cur = head;
        Node prev = null;
        int cnt = 0;
        while(cur!=null && cnt != i){
            cur = cur.next;
        }
        if(cur==null){
            return false;
        }else{
            if(cur==head){
                head=cur.next;
            }else if(cur==tail){
                tail=cur.prev;
                tail.next=null;
            }else{
                prev.next=cur.next;
            }
        }
        size--;
        return true;
    }
    
    public void appendList (List l){
        tail.next=l.head;
        tail=l.tail;
        size+=l.size();
    }
    
    public void prependList (List l){
        l.tail.next=head;
        head=l.head;
        size+=l.size();
    }

    public void print(){
        Node cur = head;
        while(cur != null){
            System.out.print(cur.val+" ");
            cur=cur.next;
        }
        System.out.println();
    }
    
   
    
    
    
}
