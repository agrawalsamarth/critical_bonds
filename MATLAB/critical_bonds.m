function critical_bonds(varargin)
close all

% (c) 24 april 2024 Martin Kroger, ETH Zurich, mk@mat.ethz.ch 

CITEAS{1} = '____________________________________________________';
CITEAS{2} = ' ';
CITEAS{3} = 'This code is part of the Supplementary Material for:';
CITEAS{4} = 'S. Agrawal, S. Galmarini, M. Kröger';
CITEAS{5} = 'Energy formulation for infinite structures: order parameter for percolation, critical bonds and power-law scaling of contact-based transport';
CITEAS{6} = 'Phys. Rev. Lett. (2024) in press';
CITEAS{7} = 'DOI: XX';
CITEAS{8} = '____________________________________________________';

% this code may not work if bonds > half box size, but it has to be tested
% this code does not erase any dangling bonds etc. for small systems
% set finalID2ID

%verbose = false;
vis     = false; 
%inputfile = 'test-config-0.txt';

switch nargin
    case 2
        inputfile = varargin(1);
        outputfile = varargin(2);
        verbose = false;
    case 3
        verbose = true;
    otherwise
        disp('call critical_bonds with one, two or three arguments:');
        disp('  critical_bonds(inputfile)                   <- writes critical bonds to result.txt');
        disp('  or'); 
        disp('  critical_bonds(inputfile,outputfile)        <- writes critical bonds to outputfile');
        disp('  or');  
        disp('  critical_bonds(inputfile,outputfile,true)   <- for verbose output');
        return;
end

if verbose, tic; end
[dim,nodes,bonds,L,ID2origID,x,b1,b2]=read_unsorted_input(inputfile,verbose);
if verbose, disp([num2str(toc) ' cpu seconds for read_input']); end
if verbose, tic; end
visualize(L,x,b1,b2,'A',vis);
if verbose, disp([num2str(toc) ' cpu seconds for visualize']); end
if verbose, tic; end
[nodes,bonds,x,b1,b2,finalID2ID]=erase_dangling_ends_and_linear_bonds(nodes,bonds,x,b1,b2,verbose);
if verbose, disp([num2str(toc) ' cpu seconds for erase_dangling_ends_and_linear_bonds']); end
if verbose, tic; end
[clusters,IDinclusterID,IDSinclusterID]=get_clusters(nodes,bonds,b1,b2,verbose);
if verbose, disp([num2str(toc) ' cpu seconds for get_clusters']); end
critical_bonds = 0; 
if verbose, tic; end
for cID=1:clusters
    [calL,bdim,NODES,BONDS,X,B1,B2] = setup_sparse_A_matrix_and_bdim_for_cluster(dim,L,x,b1,b2,cID,IDSinclusterID{cID}',verbose);
    longestBID = true;
    while isnan(longestBID)==false
        [XEQ,longestBID] = equilibrate(dim,L,calL,bdim,B1,B2);
        visualize(L,XEQ,B1,B2,['cluster ' num2str(cID)],vis);
        if isnan(longestBID)==false
            critical_bonds = critical_bonds + 1; 
            myB1 = B1(longestBID);
            myB2 = B2(longestBID);
            calL(myB1,myB1) = calL(myB1,myB1) - 1; 
            calL(myB2,myB2) = calL(myB2,myB2) - 1; 
            calL(myB1,myB2) = calL(myB1,myB2) + 1;
            calL(myB2,myB1) = calL(myB2,myB1) + 1;
            if myB1 > NODES, CHANGE_HERE_END, end
            if myB2 > NODES, CHANGE_HERE_END, end
            critical_b1(critical_bonds,:) = myB1;
            critical_b2(critical_bonds,:) = myB2;
            % erase bond, leave X unchanged, adjust B1, B2, bdim and calL
            bentry = L.*round((X(myB2,:)-X(myB1,:))./L);
            bdim(B1(longestBID),:) = bdim(myB1,:) - bentry; 
            bdim(B2(longestBID),:) = bdim(myB2,:) + bentry;
            B1(longestBID) = [];
            B2(longestBID) = [];
            BONDS = BONDS-1; 
        end
    end
end
if verbose, disp([num2str(toc) ' cpu seconds for critical bonds']); end
disp([num2str(critical_bonds) ' critical bonds']);

writematrix([critical_b1 critical_b2],outputfile,'delimiter',' ');

for k=1:length(CITEAS); disp(CITEAS{k}); end

function visualize(L,x,b1,b2,mytitle,vis)
    if vis==false; return; end
    figure; 
    plot(x(:,1),x(:,2),'ko'); hold on; 
    if length(x(:,1))<1000
        for bid=1:length(b1)
            seg = x(b2(bid),:)-x(b1(bid),:); 
            seg = seg - L.*round(seg./L); 
            plot(x(b1(bid),1)+[0 seg(1)],x(b1(bid),2)+[0 seg(2)],'k-'); 
            plot(x(b2(bid),1)-[0 seg(1)],x(b2(bid),2)-[0 seg(2)],'k-'); 
        end
    end
    axis([-L(1)/2 L(1)/2 -L(2)/2 L(2)/2]);
    title(mytitle);

function [XEQ,longestBID] = equilibrate(dim,L,calL,bdim,B1,B2)
    for d=1:dim
        XEQ(:,d) = calL\bdim(:,d);
    end
    XEQ = [XEQ;zeros(1,dim)];
    bondvectors = XEQ(B1,:)-XEQ(B2,:); 
    for d=1:dim, bondvectors(:,d) = bondvectors(:,d) - L(d)*round(bondvectors(:,d)/L(d)); end
    bondlengths = sqrt(sum(bondvectors'.^2));
    longestBID = min(find(bondlengths==max(bondlengths))); 
    longestbondlen = bondlengths(longestBID);
    if longestbondlen>1e-8
        longestBID = longestBID(1);
    else
        longestBID = nan;
    end
    disp(['longest bond length ' num2str(longestbondlen) ' at bID ' num2str(longestBID) ' /' num2str(length(B1)) ' remaining bonds']); 
    

function [calL,bdim,NODES,BONDS,X,B1,B2] = setup_sparse_A_matrix_and_bdim_for_cluster(dim,L,x,b1,b2,cID,IDs,verbose)
% for a single cluster: NODES positions X, BONDS bonds B1, B2
% IDs(n) is old ID for new ID(n)
% newID(n) is new ID for old ID(n) in 1 .. nodes
    NODES = length(IDs);
    newID = nan(length(x(:,1)),1); newID(IDs) = 1:length(IDs); 
    bIDs = unique(find(ismember(b1,IDs)+ismember(b2,IDs)>0));
    B1 = newID(b1(bIDs));
    B2 = newID(b2(bIDs));
    BONDS = length(B1); 
    if verbose, disp(['cluster #' num2str(cID) ' with ' num2str(NODES) ' nodes and ' num2str(BONDS) ' bonds.']); end
    X = x(IDs,:); 
    for d=1:dim; X(:,d) = X(:,d) - L(d)*round(X(:,d)/L(d)); end % box-centered X
    % visualize(L,X,B1,B2,'testing',vis); sssss
    bdim = zeros(NODES,dim);
    for bid=1:BONDS
        bentry = L.*round((X(B2(bid),:)-X(B1(bid),:))./L); 
        bdim(B1(bid),:) = bdim(B1(bid),:) + bentry;
        bdim(B2(bid),:) = bdim(B2(bid),:) - bentry;
    end
    A = sparse(B1,B2,ones(BONDS,1),NODES,NODES,2*BONDS+NODES) + sparse(B2,B1,ones(BONDS,1),NODES,NODES,2*BONDS+NODES);
    D = diag(sum(A)); 
    calL = D - A; 
    calL = calL(1:(NODES-1),1:(NODES-1)); 
    bdim = bdim(1:(NODES-1),:);
    
function [clusters,IDinclusterID,IDSinclusterID]=get_clusters(nodes,bonds,b1,b2,verbose)
    if bonds==0, return; end
    IDinclusterID = (1:nodes)';
    IDSinclusterID = cell(nodes,1); 
    for id=1:nodes; IDSinclusterID{id} = id; end 
    for bid=1:bonds
        cID1 = IDinclusterID(b1(bid)); 
        cID2 = IDinclusterID(b2(bid)); 
        if cID1~=cID2
            IDinclusterID(IDSinclusterID{cID2}) = cID1; 
            IDSinclusterID{cID1} = [IDSinclusterID{cID1} IDSinclusterID{cID2}];
            IDSinclusterID{cID2} = []; 
        end
    end
    clusters = 0;
    for id=1:nodes
        if length(IDSinclusterID{id})>0
            clusters = clusters + 1; 
            IDinclusterID(IDSinclusterID{id}) = clusters;
            IDSinclusterID{clusters} = IDSinclusterID{id}; 
        end
    end
    emptyCells = cellfun('isempty', IDSinclusterID);
    IDSinclusterID(all(emptyCells,2),:)=[]; 
    for cID=(clusters+1):nodes
        IDSinclusterID{cID}=[]; 
    end
    if verbose, disp([num2str(clusters) ' clusters detected.']); end


function [dim,nodes,bonds,L,ID2origID,x,b1,b2]=read_unsorted_input(inputfile,verbose)
    % after reading, we have nodes 1 .. nodes, original node IDs will
    % re-appear in the output, using ID2origID(ID+1), returned coordinates
    % are box-centered.
    % dlmread cannot simply be replaced by readmatrix due to varying number of columns
    % this reader allows for ID holes
    % remove double bonds
    A = dlmread(inputfile);  
    dim = A(1,1);
    LOHI = A(2,1:6);
    lo = LOHI([1 3 5]); 
    hi = LOHI([2 4 6]); 
    L = hi-lo; 
    nodes = A(3,1); A(1:3,:)=[]; 
    ID2origID = A(1:nodes,1);
    origID2ID = nan(max(ID2origID)+1,1);
    origID2ID(ID2origID+1) = (1:nodes);
    x = A(1:nodes,2:(dim+1));
    for d=1:dim; x(:,d)=x(:,d)-lo(d)-L(d)/2; x(:,d)=x(:,d)-L(d)*round(x(:,d)/L(d)); end
    A(1:nodes,:) = []; 
    bonds = A(1,1); A(1,:)=[]; 
    orig_b1 = A(1:bonds,1);
    orig_b2 = A(1:bonds,2); 
    b1 = origID2ID(orig_b1+1);
    b2 = origID2ID(orig_b2+1);
    b1b2 = [b1 b2];
    reverse = find(b1>b2); 
    keep = b1b2(reverse,1);
    b1b2(reverse,1) = b1b2(reverse,2);
    b1b2(reverse,2) = keep; 
    b1b2 = unique(b1b2,'rows'); 
    b1 = b1b2(:,1);
    b2 = b1b2(:,2); 
    bonds = length(b1);
    if verbose, disp([num2str(nodes) ' nodes, ' num2str(bonds) ' unique bonds, ' num2str(dim) '-dimensional periodic system, box sizes ' num2str(L(1:dim)) '.']); end

function [nodes,bonds,x,b1,b2,finalID2ID]=erase_dangling_ends_and_linear_bonds(nodes,bonds,x,b1,b2,verbose)
    if nodes<500; return; end % keep all bonds for small systems
    % keep bonds that connect over the boundary
    conn = cell(nodes,1);
    for bid=1:bonds
        conn{b1(bid)} = [conn{b1(bid)} b2(bid)]; 
        conn{b2(bid)} = [conn{b2(bid)} b1(bid)]; 
    end
    if verbose, for id=1:nodes, disp(['revised id ' num2str(id) ' has ' num2str(length(conn{id})) ' connections.']); end; end
    reduced = true;
    dangling = 0;
    linear = 0; 
    active_node = true(nodes,1);
    boundary_node = false(nodes,1);
    while reduced
        reduced = false;
        for id=1:nodes
            if length(conn{id})==1
                % erase if not connected to a dangling end and if not a boundary_node
                if boundary_node(id)==false
                    if length(conn{conn{id}})>1
                        conn{conn{id}} = conn{conn{id}}(conn{conn{id}}~=id); 
                        conn{id} = []; 
                        active_node(id) = false;
                        reduced = true;
                        dangling = dangling + 1; 
                    end
                end
            elseif length(conn{id})==2
                n1 = conn{id}(1); 
                n2 = conn{id}(2); 
                if boundary_node(id)==false
                if boundary_node(n1)==false
                if boundary_node(n2)==false
                    conn{n1} = conn{n1}(conn{n1}~=id);
                    conn{n2} = conn{n2}(conn{n2}~=id);
                    linear   = linear+2;
                    before   = length(conn{n1}); 
                    conn{n1} = unique([conn{n1} n2]); 
                    conn{n2} = unique([conn{n2} n1]);
                    after    = length(conn{n1}); 
                    if after>before
                        linear = linear-1;
                    else
                        boundary_node(n1) = true;
                        boundary_node(n2) = true;
                    end
                    conn{id} = []; 
                    active_node(id) = false;   
                    reduced = true; 
                end
                end
                end
            end
        end
    end
    bonds = bonds-dangling-linear; 
    b1 = nan(bonds,1); 
    b2 = nan(bonds,1); 
    bid = 0; 
    for id=1:nodes
        for c=1:length(conn{id})
            if id<conn{id}(c)
                bid = bid + 1; 
                b1(bid) = min(id,conn{id}(c));
                b2(bid) = max(id,conn{id}(c));
            end
        end
    end
    % now compacting nodes and bonds [b1 b2]
    finalID2ID = find(active_node==true);
    final_nodes = length(finalID2ID);
    finalID2ID = sortrows([finalID2ID (1:final_nodes)'],1);
    finalID2ID = finalID2ID(:,1);
    ID2finalID = nan(nodes,1);
    ID2finalID(finalID2ID) = 1:final_nodes; 
    b1 = ID2finalID(b1);
    b2 = ID2finalID(b2);
    x = x(finalID2ID,:); 
    nodes = length(x(:,1));
    bonds = length(b1); 
    if verbose, disp([num2str(nodes) ' nodes and ' num2str(bonds) ' bonds considered in the analysis.']); end